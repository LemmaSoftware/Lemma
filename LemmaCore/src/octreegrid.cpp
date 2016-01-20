/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/19/2010
  @version  $Id: octreegrid.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "octreegrid.h"

#ifdef  LEMMAUSEVTK
namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
            const OctreeGrid &ob) {
        stream << *(Grid*)(&ob);
        return stream;
    }


    OctreeGrid::OctreeGrid(const std::string &name) :
        Grid(name),
        Size(Vector3r::Zero()), Origin(Vector3r::Zero()),
        step(Vector3r::Zero()), cpos(Vector3r::Zero()),
        level(0), maxlevel(12), index(0) ,counter(0), cvol(0), tvol(0), tol(0),
        SenseKernel( NULL ),
        Cubes(ReceiverCubes::New()),
        Model1D(NULL),
        Octree(vtkHyperOctree::New() ),
        Cursor(Octree->NewCellCursor()),
        OctSamp(vtkHyperOctreeSampleFunction::New() ),
        #ifdef  LEMMA_SINGLE_PRECISION
        KernelArray(vtkFloatArray::New())
        #else      // -----  not LEMMA_SINGLE_PRECISION  -----
        KernelArray(vtkDoubleArray::New())
        #endif     // -----  not LEMMA_SINGLE_PRECISION  -----
    {
        // 8 children plus root
        Cubes->SetNumberOfReceivers(9);
        // TODO point caclulation fails if origin is < 0.
        // Generalise, TODO add method to set Origin Depth
        Octree->SetDimension(3);
        Octree->SetOrigin(0,0,0);
        Origin << 0, 0, 0;
    }

    OctreeGrid::~OctreeGrid() {

        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences( this );

        // Bug here
        if (SenseKernel != NULL) {
            //SenseKernel->SetFieldCubes(NULL);
            SenseKernel->DetachFrom(this);
            SenseKernel = NULL;
        }

        if (Model1D != NULL) {
            Model1D->DetachFrom(this);
            Model1D = NULL;
        }

        Cubes->Delete();

        // With VTK 5.2 memory issues, unconditional jump.
        // TODO check with more recent versions.
        Cursor->Delete();
        Octree->Delete();
        OctSamp->Delete();
        KernelArray->Delete();
    }

    OctreeGrid * OctreeGrid::New() {
        OctreeGrid* Obj = new OctreeGrid("OctreeGrid");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void OctreeGrid::Delete() {
        this->DetachFrom(this);
    }

    void OctreeGrid::Release() {
        delete this;
    }

    // ====================  OPERATIONS    =======================

    void OctreeGrid::GenerateMesh(const Real &tolerance) {
        this->tol = tolerance;
        KernelSum = 0.;
        Cursor->ToRoot();
        Cubes->SetNumberOfReceivers(8);
        EvaluateKids(1e9); // Large initial number don't waste time actually computing
        //EvaluateKids();
        //std::cout << "Kernel Sum from Generate Mesh "
        //    << std::real(KernelSum) << "\t" << std::imag(KernelSum) << std::endl;

        // TODO uncomment
        SetLeafDataFromGridCreation();
    }

    void OctreeGrid::GenerateMesh(const Real &tolerance,
        vtkImplicitFunction* Impl, const Real& impltol) {

        this->tol = tolerance;
        KernelSum = 0.;

        // Initial cut based on implicit function
        //Octree->Delete();
        Cursor->Delete();

        std::cout << "Sampling model onto Octree grid " << std::endl;

        if (OctSamp->GetImplicitFunction() != Impl) {
            // TODO maybe should  have accessors for some of this?
            OctSamp->SetImplicitFunction(Impl);
            OctSamp->SetDimension(3);
            OctSamp->SetMinLevels(2);
            OctSamp->SetLevels(10);
            OctSamp->SetThreshold(impltol);
            OctSamp->SetOrigin(Origin[0], Origin[1], Origin[2]);
            OctSamp->SetSize(Size[0], Size[1], Size[2]);
            OctSamp->Update();
            //this->Octree->DeepCopy( OctSamp->GetOutput() ); //->Copy();
            std::cout << "Octree Leaves from implicit function sampling "
                      << OctSamp->GetOutput()->GetNumberOfLeaves() << std::endl;
        }

        // OK, now evaluate kernel function
        this->Octree->DeepCopy(OctSamp->GetOutput());//->DeepCopy();
        this->Cursor = Octree->NewCellCursor();

        //FillMeshWithKernel(Impl);
        counter=0;
        //EvaluateKernelOnMesh();
        //FillMeshWithKernel(Impl);

        this->tol = tolerance;
        KernelSum = 0.;
        Cursor->ToRoot();
        //EvaluateKids( Impl  );

        // TODO -> Instead of going to root, I need to loop over the leaves and
        // call EvaluateKids on each Leaf of above Octree.
        TraverseLeaves(Impl);

        std::cout << "\nKernel Sum from Generate Mesh "
            << std::real(KernelSum) << "\t" << std::imag(KernelSum) << std::endl;
        std::cout << "Octree Leaves from second " << Octree->GetNumberOfLeaves() << std::endl;

        // TODO uncomment
        SetLeafDataFromGridCreation();

        //OctSamp->Delete();
        Octree = vtkHyperOctree::New();
        Octree->SetDimension(3);
        Octree->SetOrigin(0,0,0);
    }

    void OctreeGrid::GenerateMesh(const Real &tolerance, vtkImplicitFunction* Impl, vtkHyperOctreeSampleFunction* OctSamp2) {

        this->tol = tolerance;
        KernelSum = 0.;
        //Octree->Delete();
        Cursor->Delete();

        std::cout << "Sampling kernel onto Octree grid " << std::endl;
        if (OctSamp2->GetImplicitFunction() != Impl) {
            cerr << "Octree sample and Implicit function do not match! in octreegrid.cpp\n";
            exit(1);
        }

        // OK, now evaluate kernel function
        //this->Octree->DeepCopy(OctSamp2->GetOutput());//->DeepCopy();
        this->Cursor = this->Octree->NewCellCursor();
        this->Cursor->ToRoot();

        vtkHyperOctreeCursor* LeaderCurse = OctSamp2->GetOutput()->NewCellCursor();
        LeaderCurse->ToRoot();
        FollowExistingTree(OctSamp2->GetOutput(), LeaderCurse, Impl);
        SetLeafDataFromGridCreation();

//         //FillMeshWithKernel(Impl);
//         counter=0;
//
//         // TODO -> Instead of going to root, I need to loop over the leaves and
//         // call EvaluateKids on each Leaf of above Octree.
//         //std::cout << *OctSamp2->GetOutput()->GetLeafData()->GetScalars() << std::endl;
//         //SampleArray = OctSamp2->GetOutput()->GetLeafData()->GetScalars();
//         Octree->SetDualGridFlag(1);
//         std::cout << "sampling function " << std::endl;
// //         vtkHyperOctree* sf = vtkHyperOctree::New();
// //         sf->DeepCopy(this->Octree);
// //         vtkHyperOctreeCursor* sfc = sf->NewCellCursor();
// //         sfc->ToRoot();
// //         sf->SubdivideLeaf(sfc);
// //         for (int child=0; child<sfc->GetNumberOfChildren(); ++child) {
// //             sfc->ToChild(child);
// //             sf->SubdivideLeaf(sfc);
// //             sfc->ToParent();
// //         }
// //         //SampleFunction(Impl);
// //         Octree->DeepCopy(sf);
//         //vtkHyperOctreeCursor *tcurse = Cursor->Clone();
//         Octree->SubdivideLeaf(Cursor);
//         //tcurse->ToSameNode(Cursor);    // Save location for when we come out
//         for (int child=0; child<Cursor->GetNumberOfChildren(); ++child) {
//             Cursor->ToChild(child);
//             Octree->SubdivideLeaf(Cursor);
//             for (int i=0; i<8; ++i) {
//                 Cursor->ToChild(i);
//                 Real* p2 = Octree->GetPoint(Cursor->GetLeafId());
// //                 if (i==0) {
// //                     Octree->SubdivideLeaf(Cursor);
// //                     for (int i=0; i<8; ++i) {
// //                     Cursor->ToChild(i);
// //                     Real* p2 = Octree->GetPoint(Cursor->GetLeafId());
// //                     Cursor->ToParent();
// //                 }
// //                 }
//                 Cursor->ToParent();
//             }
//             Cursor->ToParent();
//         }
//         Cursor->ToChild(0);
//         Cursor->ToChild(0);
//             Octree->SubdivideLeaf(Cursor);
//             for (int i=0; i<8; ++i) {
//                 Cursor->ToChild(i);
//                 Real* p2 = Octree->GetPoint(Cursor->GetLeafId());
//                 Cursor->ToParent();
//             }
//         Cursor->ToRoot();
//         //Cursor->ToSameNode(tcurse);
//         //tcurse->Delete();
//         //this->Cursor->ToParent();
//         std::cout << "Traversing Leaves " << std::endl;
//         //Cursor->ToRoot();
//         //this->Octree->GetLeafData()->GetScalars()->Reset(  );
//         TraverseLeaves(Impl);   // split but don't fill
//         //TraverseAndCall(Impl);
//         //ReTraverseLeaves(Impl);
//         /*
//         std::cout << "\nKernel Sum from Generate Mesh "
//             << std::real(KernelSum) << "\t" << std::imag(KernelSum) << std::endl;
//         std::cout << "Octree Leaves from second " << Octree->GetNumberOfLeaves() << std::endl;
//         */
//         // TODO uncomment
//         std::cout << "counter " << counter << std::endl;
//         SetLeafDataFromGridCreation();
//         this->Cursor->Delete();
    }

    Complex OctreeGrid::EvaluateKernelOnMesh( ) {
        KernelSum = 0.;
        //Cursor->ToRoot();
        FillGrid();
        //std::cout << std::real(KernelSum) << "\t" << std::imag(KernelSum) << std::endl;
        //SetLeafDataFromGridCreation();
        return KernelSum;
    }

    void OctreeGrid::FillMeshWithKernel() {
        this->KernelArray->SetNumberOfTuples(Octree->GetNumberOfLeaves());
        FillGrid();
        this->Octree->GetLeafData()->SetScalars( KernelArray );
    }

    void OctreeGrid::FillMeshWithKernel(vtkImplicitFunction* Impl) {
        this->KernelArray->SetNumberOfTuples(Octree->GetNumberOfLeaves());
        std::cout << "Octree Leaves " << Octree->GetNumberOfLeaves() << std::endl;
        //Cursor->ToRoot();
        FillGrid( Impl );
        //this->Octree->GetLeafData()->SetScalars( KernelArray );
    }

    void OctreeGrid::ReceiverCubeRepresentation() {
        tvol = 0;
        Cursor->ToRoot();
        Cubes->SetNumberOfReceivers( Octree->GetNumberOfLeaves() );
        AssembleReceiverCubes();
        //std::cout << "Total volume " << tvol << std::endl;
    }

    ReceiverCubes* OctreeGrid::GetReceiverCubes() {
        return this->Cubes;
    }

    Complex OctreeGrid::GetKernelSum() {
        return this->KernelSum;
    }

    void OctreeGrid::CursorToRoot() {
        Cursor->ToRoot();
    }

    void OctreeGrid::AssembleReceiverCubes() {

        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;
        Vector3r leafstep = step;

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        //  split if needed
        if (Cursor->CurrentIsLeaf())  {
            // Set this cube
            Cubes->SetLocation(Cursor->GetLeafId(), cpos);
            Cubes->SetLength(Cursor->GetLeafId(), leafstep);
            tvol += leafstep[0]*leafstep[1]*leafstep[2];
        } else {
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                Cursor->ToChild(child);
                AssembleReceiverCubes( );
                Cursor->ToParent( );
            }
        }
        tcurse->Delete();
    }

    void OctreeGrid::FillGrid() {
        Cubes->SetNumberOfReceivers(1);
        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;
        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);
        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;
        while (tcurse->GetCurrentLevel() > 0) {
            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));
            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }
        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));
        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];
        cpos += Posadd.row(index);
        Cubes->SetLocation(0, cpos);
        Cubes->SetLength(0, step);
        //////////////////////////////////////////////
        // Next level step
        step  = (Size).array() / (std::pow(2.,(level+1)));
        //Vector3r pos   =  cpos - step/2.;
        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];
        // Evaluate kernel
        Cubes->ClearFields();
        VectorXcr f = SenseKernel->ComputeSensitivity();
        //Real val   = cvol*std::abs(f(0));
        //  split if needed
        if (Cursor->CurrentIsLeaf())  {
            if (f(0) == f(0)) {
                KernelSum += f(0); // catch NaNs
                KernelArray->InsertValue(Cursor->GetLeafId(), std::abs(f(0)));
            }
            else {
                std::cerr << "f(0) " << f(0) << std::endl;
                KernelArray->InsertValue(Cursor->GetLeafId(), 0);
            }
        } else {
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                //Vector3r cp = pos + Posadd.row(child).transpose();
                Cursor->ToChild(child);
                FillGrid( );
                Cursor->ToParent();
            }

        }
        tcurse->Delete();
        Cubes->SetNumberOfReceivers(9);
    }

    void OctreeGrid::FillGrid(vtkImplicitFunction* Impl) {
        std::cout << "counter " << counter << std::endl;
        ++ counter;
        if (Cursor->CurrentIsLeaf())  {
            double  pcords[3];
            double  x[3];
            double  *w(NULL);
            int id;
            Octree->GetCell(Cursor->GetLeafId())->GetParametricCenter(pcords);
            Octree->GetCell(Cursor->GetLeafId())->EvaluateLocation(id, pcords, x, w);
            KernelArray->InsertValue(Cursor->GetLeafId(), x[2]);
            cout << "pcords " << x[0] << "\t" << x[1] << "\t" << x[2] << std::endl;
        } else {
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                Cursor->ToChild(child);
                FillGrid(Impl);
                Cursor->ToParent();
            }
        }
//         Cubes->SetNumberOfReceivers(1);
//         level = Cursor->GetCurrentLevel();
//         index = Cursor->GetChildIndex() ;
//         // current position
//         // backtrack through parents calculate
//         // TODO bug, if origin is not (0,0,0) this breaks :(
//         vtkHyperOctreeCursor *tcurse = Cursor->Clone();
//         tcurse->ToSameNode(Cursor);
//         step  = ((Size-Origin).array() / std::pow(2.,level));
//         cvol  = step[0]*step[1]*step[2];
//         cpos  = Origin + step/2;
//         while (tcurse->GetCurrentLevel() > 0) {
//             int lev   = tcurse->GetCurrentLevel();
//             index = tcurse->GetChildIndex();
//             step  = ((Size-Origin) / std::pow(2.,lev));
//             Posadd <<       0,       0,       0,
//                       step[0],       0,       0,
//                             0, step[1],       0,
//                       step[0], step[1],       0,
//                             0,       0, step[2],
//                       step[0],       0, step[2],
//                             0, step[1], step[2],
//                       step[0], step[1], step[2];
//             cpos  += Posadd.row(index);
//             tcurse->ToParent();
//         }
//         // Root level
//         int lev   = tcurse->GetCurrentLevel();
//         index = tcurse->GetChildIndex();
//         step  = (Size-Origin).array() / (std::pow(2.,lev));
//         Posadd <<       0,       0,       0,
//                   step[0],       0,       0,
//                         0, step[1],       0,
//                   step[0], step[1],       0,
//                         0,       0, step[2],
//                   step[0],       0, step[2],
//                         0, step[1], step[2],
//                   step[0], step[1], step[2];
//         cpos += Posadd.row(index);
//         Cubes->SetLocation(0, cpos);
//         Cubes->SetLength(0, step);
//         //////////////////////////////////////////////
//         // Next level step
//         step  = (Size-Origin).array() / (std::pow(2.,(level+1)));
//         //Vector3r pos   =  cpos - step/2.;
//         Posadd <<       0,       0,       0,
//                   step[0],       0,       0,
//                         0, step[1],       0,
//                   step[0], step[1],       0,
//                         0,       0, step[2],
//                   step[0],       0, step[2],
//                         0, step[1], step[2],
//                   step[0], step[1], step[2];
//         // Evaluate kernel
//         Cubes->ClearFields();
//         VectorXcr f = SenseKernel->ComputeSensitivity(Impl);
//         //Real val   = cvol*std::abs(f(0));
//         //  split if needed
//         if (Cursor->CurrentIsLeaf())  {
//             if (f(0) == f(0)) {
//                 KernelSum += f(0); // catch NaNs
//                 KernelArray->InsertValue(Cursor->GetLeafId(), std::abs(f(0)));
//             }
//             else {
//                 std::cerr << "f(0) " << f(0) << std::endl;
//                 KernelArray->InsertValue(Cursor->GetLeafId(), 0);
//             }
//         } else {
//             // Evaluate function at cell centre
//             for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
//                 //Vector3r cp = pos + Posadd.row(child).transpose();
//                 Cursor->ToChild(child);
//                 FillGrid(Impl);
//                 Cursor->ToParent();
//             }
//         }
//         tcurse->Delete();
//         Cubes->SetNumberOfReceivers(9);
    }

    void OctreeGrid::SampleScalar() {

        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        // ######################################
        // # Next level step
        step  = (Size).array() / (std::pow(2.,(level+1)));
        //Vector3r pos   =  cpos - step/2.;
        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];


        //  split if needed
        if (Cursor->CurrentIsLeaf())  {
            KernelArray->InsertValue(Cursor->GetLeafId(),
                SampleScalarFromLayeredEarthFD(GetLayerInt(cpos(2))));
        } else {
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                Cursor->ToChild(child);
                SampleScalar( );
                Cursor->ToParent();
            }

        }
        tcurse->Delete();
    }

    void OctreeGrid::GetPosition( Real* p ) {
        Real ratio=1.0/(1<<(Cursor->GetCurrentLevel()));
        //step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
        p[0]=(Cursor->GetIndex(0)+.5)*ratio*this->Size[0]+this->Origin[0] ;//+ .5*step[0];
        p[1]=(Cursor->GetIndex(1)+.5)*ratio*this->Size[1]+this->Origin[1] ;//+ .5*step[1];
        p[2]=(Cursor->GetIndex(2)+.5)*ratio*this->Size[2]+this->Origin[2] ;//+ .5*step[2];
    }

    void OctreeGrid::SampleFunction( vtkImplicitFunction* Impl ) {

        assert(Cursor->CurrentIsLeaf());

        Real p[3];


        this->GetPosition(p);
        std::cout << "Before At " << p[0] << "\t" << p[1] << "\t" << p[2] << std::endl;
        //Octree->GetPoint(Cursor->GetLeafId(), p);

        //Octree->GetPoint(Cursor->GetLeafId(), p);
        //Real value = Impl->FunctionValue(p);

        Octree->SubdivideLeaf(Cursor);
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor); // Save location for when we come out

        for (int child=0; child < Cursor->GetNumberOfChildren(); ++child) {
            Cursor->ToChild(child);
//             //Octree->GetPoint(Cursor->GetLeafId(), p);
//             //this->GetPosition(p);
//             //Real valueSm = Impl->FunctionValue(p);
//             //if (std::abs(value - valueSm) > 1e-1) {
//                 //std::cout << "tcurse\t" << tcurse->GetLeafId() << "\t" << tcurse->GetCurrentLevel() << std::endl;
//                 //this->GetPosition(p);
//
//
//             //    SampleFunction(Impl);
//                 //std::cout << "Dual Grid Flag after subdivide " << Octree->GetDualGridFlag() << std::endl;
// //                 for (int child=0; child<Cursor->GetNumberOfChildren(); ++child) {
// //                     Cursor->ToChild(child);
// //                     Octree->GetPoint(Cursor->GetLeafId(), p);
// //                     leafdata.push_back( p[2] ); // was value
// //                     leafids.push_back(Cursor->GetLeafId());
// //                     Cursor->ToParent();
// //                 }
//                 //EvaluateKids(Impl);          // don't need to use Impl, will be constant here on
//                 //Octree->SetDualGridFlag(1);
//                 //EvaluateKids(value, Impl);          // don't need to use Impl, will be constant here on
//                 //leafdata.push_back( p2[0] );
//                 //leafids.push_back(Cursor->GetLeafId());
//             //}
            //Cursor->ToParent( );
            Cursor->ToSameNode(tcurse);
//             //this->GetPosition(p);
//             //std::cout << "After At " << p[0] << "\t" << p[1] << "\t" << p[2] << std::endl;
//             //std::cout << "GOTO tcurse\t" << tcurse->GetLeafId() << "\t" << tcurse->GetCurrentLevel() << std::endl;
        }
        tcurse->Delete();

    }

    void OctreeGrid::FollowExistingTree( vtkHyperOctree* Leader, vtkHyperOctreeCursor* LeaderCursor, vtkImplicitFunction* Impl ) {

        assert(!LeaderCursor->CurrentIsLeaf());
        assert("Follow existing, CurrentIsLeaf" && Cursor->CurrentIsLeaf());

        Real p[3];
        // match Leader topology
        Octree->SubdivideLeaf(Cursor);
        for (int child=0; child<8; ++child) {
            LeaderCursor->ToChild(child);
            Cursor->ToChild(child);
            if (LeaderCursor->CurrentIsLeaf()) {
                GetPosition(p);
                // Fill-in Octree
                Real value = Impl->FunctionValue(p);
                EvaluateKids(value, Impl);
                //Octree->SubdivideLeaf(Cursor);
            } else {
                FollowExistingTree(Leader, LeaderCursor, Impl);
            }
            Cursor->ToParent();
            LeaderCursor->ToParent();
        }
    }

    void OctreeGrid::TraverseLeaves(vtkImplicitFunction* Impl) {
        //static int first = true;
        assert(!Cursor->CurrentIsLeaf());
        Real p[3];
        //Real* p2;
        //Real p2[3];
        //vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        //tcurse->ToSameNode(Cursor); // Save location for when we come out
        //vtkIdList *ptIds = vtkIdList::New();
        //this->GetPosition(p);
        //std::cout << "Before At " << p[0] << "\t" << p[1] << "\t" << p[2] << std::endl;
        for (vtkIdType child=0; child < Cursor->GetNumberOfChildren(); ++child) {
            Cursor->ToChild(child);
            if (Cursor->CurrentIsLeaf()) {
                //std::cout << "tcurse\t" << tcurse->GetLeafId() << "\t" << tcurse->GetCurrentLevel() << std::endl;
                Octree->Modified();
                this->GetPosition(p);
                step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
                //vtkGenericCell *cell;
                //Octree->GetCell(Cursor->GetLeafId(),
                //             vtkGenericCell *cell);
                //p[2] = cell->GetPoint(0);

                //Octree->GetCellPoints(Cursor->GetLeafId(), ptIds);
                //p2 = Octree->GetPoint(Cursor->GetLeafId());
                //Octree->GetPoint(Cursor->GetLeafId(), p2 );
                //std::cout << p[0] - p2[0] << "\t" << p[1] - p2[1] << "\t"
                //         << p[2] - p2[2] << std::endl;
                //Real value=Impl->FunctionValue(p);
                //std::cout << "Dual Grid Flag b4 subdivide " << Octree->GetDualGridFlag() << std::endl;
                //Octree->SetDualGridFlag(0);
                /*
                if (first == true) {
                Octree->SubdivideLeaf(Cursor);
                //std::cout << "Dual Grid Flag after subdivide " << Octree->GetDualGridFlag() << std::endl;
                for (vtkIdType child=0; child<Cursor->GetNumberOfChildren(); ++child) {
                    //vtkHyperOctreeCursor *ttcurse = Cursor->Clone();
                    //ttcurse->ToSameNode(Cursor); // Save location for when we come out
                    Cursor->ToChild(child);
                    //Octree->GetPoint(Cursor->GetLeafId(), p);
                    //leafdata.push_back( p2[2] ); // was value
                    //leafids.push_back(Cursor->GetLeafId());
                    Cursor->ToParent();
                    //Cursor->ToSameNode(ttcurse);
                    //ttcurse->Delete();
                }
                first = false;
                } else {
                */
                //EvaluateKids(Impl);          // don't need to use Impl, will be constant here on
                //Octree->SetDualGridFlag(1);
                //EvaluateKids(value, Impl);          // don't need to use Impl, will be constant here on
                leafdata.push_back( p[1] );
                ++counter;
                leafids.push_back(Cursor->GetLeafId());

                //vtkIdType id=Cursor->GetLeafId();
                //Octree->GetLeafData()->GetScalars()->InsertTuple1(id, p[2]);

                //}
            } else {
                TraverseLeaves(Impl);
            }
            //Cursor->ToSameNode(tcurse);
            Cursor->ToParent();
            //this->GetPosition(p);
            //std::cout << "After At " << p[0] << "\t" << p[1] << "\t" << p[2] << std::endl;
            //std::cout << "GOTO tcurse\t" << tcurse->GetLeafId() << "\t" << tcurse->GetCurrentLevel() << std::endl;
        }
        //tcurse->Delete();
    }

    void OctreeGrid::TraverseAndCall(InverseSolver* Inverse) {

        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;
        Vector3r leafstep = step;

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        //  split if needed
        if (Cursor->CurrentIsLeaf())  {
            // Set this cube
            Inverse->FillInG( cpos, leafstep );
            tvol += leafstep[0]*leafstep[1]*leafstep[2];
        } else {
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                Cursor->ToChild(child);
                TraverseAndCall(Inverse);
                Cursor->ToParent( );
            }
        }
        tcurse->Delete();
    }

    void OctreeGrid::TraverseAndCall( vtkImplicitFunction* Impl ) {

        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;
        Vector3r leafstep = step;

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        //  split if needed
        if (Cursor->CurrentIsLeaf())  {
            // Set this cube
            leafdata.push_back( cpos[2] ); // was value
            leafids.push_back(Cursor->GetLeafId());
            tvol += leafstep[0]*leafstep[1]*leafstep[2];
        } else {
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                Cursor->ToChild(child);
                TraverseAndCall(Impl);
                Cursor->ToParent( );
            }
        }
        tcurse->Delete();
    }

    void OctreeGrid::EvaluateKids( Complex kval ) {

        assert("Evaluate Kids pre" && Cursor->CurrentIsLeaf());
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();

        Real p[3];

        Octree->SubdivideLeaf(Cursor);
        tcurse->ToSameNode(Cursor);

        std::cout << "\rPredivide Leaf count: " << Octree->GetNumberOfLeaves();
        //std::cout.flush();

        for (int child=0; child<8; ++child) {
            Cursor->ToChild(child);
            assert(Cursor->CurrentIsLeaf());

            // Build cube
            GetPosition(p);
            cpos <<  p[0], p[1], p[2];
            step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
            Cubes->SetLocation(child, cpos);
            Cubes->SetLength(child, step);
            //std::cout << "child " << child << " cpos\t" << cpos.transpose() << std::endl;
            //std::cout << "child " << child << " step\t" << step.transpose() << std::endl;
            Cursor->ToSameNode(tcurse);
        }

        // make calculation
        Cubes->ClearFields();
        VectorXcr f = SenseKernel->ComputeSensitivity();

        if ( std::abs(std::abs(kval) - std::abs(f.array().abs().sum())) <= tol ||
            Cursor->GetCurrentLevel() >= maxlevel ) {
    	    // stop subdividing, save result
    	    for (int child=0; child < 8; ++ child) {
    	        Cursor->ToChild(child);
    	        leafdata.push_back( std::abs(f(child)) / Cubes->GetVolume(child) );
    	        // TODO fval is just a test
    	        //leafdata.push_back( fval );
    	        leafids.push_back(Cursor->GetLeafId());
    	        KernelSum += f(child);
    	        Cursor->ToParent();
            }
    	    return;
        }
        //std::cout << std::abs(kval) << "\t" <<
        //         std::abs(f.array().abs().sum()) << "\t" << tol << std::endl;
        for (int child=0; child < 8; ++ child) {
            //std::cout << "Down the rabit hole " <<std::endl;
            Cursor->ToChild(child);
            EvaluateKids( f(child) );
            //Cursor->ToParent();
            Cursor->ToSameNode(tcurse);
        }

        tcurse->Delete();

    }

    void OctreeGrid::EvaluateKids() {
        // TODO, rewrite to not dive into next virtual level do calculation, and then split.
        // instead pre-emptively split do analysis and then decide if it should be split again.
        // Also, start passing the kernel evaluation from the parent to the kid, so that does not
        // need to be repeated. We can purge a lot of recalculation this way.
        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;

        // step at this level
        Cubes->SetLength(0, step);
        //Real stepz0 = step[2];

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        // If this cube has any intersections in the 90% middle, then jump to
        // subdividing this up, TODO don't hard code this all in
        // NOTE: This works but it is very slow. Causes a TON of cells to be
        // created near interface if that interface happens to be in an
        // invonventien place. Instead moving this to kernel.
//      if ( Model1D->GetLayerAtThisDepth(cpos(2) - .45*stepz0) !=
//           Model1D->GetLayerAtThisDepth(cpos(2) + .45*stepz0) && stepz0 > 10) {
//              Octree->SubdivideLeaf(Cursor);
//              // Evaluate function at cell centre
//              for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
//                  //Vector3r cp = pos + Posadd.row(child).transpose();
//                  Cursor->ToChild(child);
//                  EvaluateKids( );
//                  Cursor->ToParent();
//              }
//      }
        Cubes->SetLocation(0, cpos);

        /////////////////////////////////////////
        // Next level step
        step  = (Size).array() / (std::pow(2.,(level+1)));
        Vector3r pos   =  cpos - step/2.;
        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        Real sum = 0;
        Real vv = 0;

        for (int child=0; child<8; ++ child) {
            // This cell centre and volume
            Vector3r cp = pos + Posadd.row(child).transpose();
            Cubes->SetLocation(child+1, cp);
            Cubes->SetLength(child+1, step);
        }

        // Evaluate kernel
        Cubes->ClearFields();
        VectorXcr f = SenseKernel->ComputeSensitivity();
        Real val    = std::abs(f(0)); // *cvol not needed, kernel does it

        Real vol = step[0]*step[1]*step[2];
        for (int child=0; child<8; ++ child) {
            vv  += vol;
            sum += std::abs(f(child+1)); // *vol not needed, kernel does it.
        }

        // split if needed
        //std::cout << "f" << f.transpose() << std::endl;
        //std::cout << "val " << sum << "\t" << val << "dif " << std::abs(sum - val)
        //      << std::endl;
        //std::cout << "level " << level << "\t" << index << std::endl;

        // Volume based test
        //if ( (Cursor->CurrentIsLeaf() && std::abs(sum-val) > tol) || level < 4)  {
        if ( (Cursor->CurrentIsLeaf() && std::abs(sum-val) > tol) || level < 3)  {

        // Point difference
        //if ( Cursor->CurrentIsLeaf() &&
        //      (f.tail<8>().array()-f(0)).array().abs().maxCoeff() > tol &&
        //      level < maxlevel ) {

            //std::cout << "Subdividing\n";
        //if ( Cursor->CurrentIsLeaf() &&
        //((f.tail<8>().array()-f(0)).array().abs().maxCoeff()).cast<Real>().eval()[0] > tol )  {
            Octree->SubdivideLeaf(Cursor);
            std::cout << "\r       Leaf count: " << Octree->GetNumberOfLeaves();
            std::cout.flush();
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                //Vector3r cp = pos + Posadd.row(child).transpose();
                Cursor->ToChild(child);
                EvaluateKids( ); // TODO EvaluateKids(f(child+1));
                Cursor->ToParent();
            }
        } else if (Cursor->CurrentIsLeaf()) {

            // For theta imaging
            //Real intpart;
            //leafdata.push_back( 2.*std::abs(modf(std::abs(f(0))/(2.*PI), &intpart)-.5) );
            leafdata.push_back( std::abs(f(0)) / Cubes->GetVolume(0) );
            //Octree->CollapseTerminalNode(Cursor);
            leafids.push_back( Cursor->GetLeafId() );
            KernelSum += f(0);
            //std::cout << ( cvol*std::abs(f(0))  ) << "\n";
        }
        tcurse->Delete();
    }

    void OctreeGrid::EvaluateKids(vtkImplicitFunction* Impl) {

        level = Cursor->GetCurrentLevel();
        index = Cursor->GetChildIndex() ;

        // current position
        // backtrack through parents calculate
        // TODO bug, if origin is not (0,0,0) this breaks :(
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();
        tcurse->ToSameNode(Cursor);

        step  = ((Size).array() / std::pow(2.,level));
        cvol  = step[0]*step[1]*step[2];
        cpos  = Origin + step/2;

        // step at this level
        Cubes->SetLength(0, step);

        while (tcurse->GetCurrentLevel() > 0) {

            int lev   = tcurse->GetCurrentLevel();
            index = tcurse->GetChildIndex();
            step  = ((Size) / std::pow(2.,lev));

            Posadd <<       0,       0,       0,
                      step[0],       0,       0,
                            0, step[1],       0,
                      step[0], step[1],       0,
                            0,       0, step[2],
                      step[0],       0, step[2],
                            0, step[1], step[2],
                      step[0], step[1], step[2];

            cpos  += Posadd.row(index);
            tcurse->ToParent();
        }

        // Root level
        int lev   = tcurse->GetCurrentLevel();
        index = tcurse->GetChildIndex();
        step  = (Size).array() / (std::pow(2.,lev));

        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        cpos += Posadd.row(index);

        Cubes->SetLocation(0, cpos);

        /////////////////////////////////////////
        // Next level step
        step  = (Size).array() / (std::pow(2.,(level+1)));
        Vector3r pos   =  cpos - step/2.;
        Posadd <<       0,       0,       0,
                  step[0],       0,       0,
                        0, step[1],       0,
                  step[0], step[1],       0,
                        0,       0, step[2],
                  step[0],       0, step[2],
                        0, step[1], step[2],
                  step[0], step[1], step[2];

        Real sum = 0;
        Real vv = 0;

        for (int child=0; child<8; ++ child) {
            // This cell centre and volume
            Vector3r cp = pos + Posadd.row(child).transpose();
            Cubes->SetLocation(child+1, cp);
            Cubes->SetLength(child+1, step);
        }

        // Evaluate kernel
        Cubes->ClearFields();
        VectorXcr f = SenseKernel->ComputeSensitivity(Impl);
        //std::cout << "f " << f.transpose() << std::endl;
        Real val    = std::abs(f(0)); // *cvol not needed, kernel does it
        Real vol = step[0]*step[1]*step[2];
        for (int child=0; child<8; ++ child) {
            vv  += vol;
            sum += std::abs(f(child+1)); // *vol not needed, kernel does it.
        }

        // split if needed

        // Volume based test
        if ( (Cursor->CurrentIsLeaf() && std::abs(sum-val) > tol) || level < 3)  {
            Octree->SubdivideLeaf(Cursor);
            std::cout << "\rLeaf count: " << Octree->GetNumberOfLeaves();
            std::cout.flush();
            // Evaluate function at cell centre
            for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
                //Vector3r cp = pos + Posadd.row(child).transpose();
                Cursor->ToChild(child);
                EvaluateKids( Impl );
                Cursor->ToParent();
            }
        } else if (Cursor->CurrentIsLeaf()) {
            // For theta imaging
            //Real intpart;
            //leafdata.push_back( 2.*std::abs(modf(std::abs(f(0))/(2.*PI), &intpart)-.5) );
            leafdata.push_back( std::abs(f(0)) / Cubes->GetVolume(0) );
            //leafdata.push_back( Impl->EvaluateFunction(cpos[0],cpos[1], cpos[2]) );
            //leafdata.push_back( cpos[1] );
            leafids.push_back(Cursor->GetLeafId());
            KernelSum += f(0);
            //if ( std::abs(f(0)) > 1e-30 ) {
            //    std::cout.precision(12);
            //    std::cout << "leaf data " << sum << "\t" << f(0) << "\t" << Cubes->GetVolume(0) << std::endl;
            //}
        } // else {
          //  for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
          //      //Vector3r cp = pos + Posadd.row(child).transpose();
          //      Cursor->ToChild(child);
          //      EvaluateKids( Impl );
          //      Cursor->ToParent();
          //  }
        //}
        tcurse->Delete();
    }

    void OctreeGrid::EvaluateKids(const Real& fval, vtkImplicitFunction* Impl) {

        assert("Evaluate Kids pre" && Cursor->CurrentIsLeaf());
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();

        VectorXr Values(9);
        Values(0) = fval;
        Real p[3];

        // at this level
        GetPosition(p);
        cpos <<  p[0], p[1], p[2];
        step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
        Cubes->SetLocation(0, cpos);
        Cubes->SetLength(0, step);
        Octree->SubdivideLeaf(Cursor);
        tcurse->ToSameNode(Cursor);

        std::cout << "\r       Leaf count: " << Octree->GetNumberOfLeaves();

        // TODO can this be parallelised? I don't know if call to FunctionValue is thread safe
        for (int child=0; child<Cursor->GetNumberOfChildren(); ++child) {
            Cursor->ToChild(child);
            assert(Cursor->CurrentIsLeaf());

            // Build cube
            GetPosition(p);
            Values[child+1] = Impl->FunctionValue(p);
            cpos <<  p[0], p[1], p[2];
            step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
            Cubes->SetLocation(child+1, cpos);
            Cubes->SetLength(child+1, step);

            Cursor->ToSameNode(tcurse);
        }

        // make calculation
        Cubes->ClearFields();
        // TODO if Values don't change much, we can set them static and use old logic
        VectorXcr f = SenseKernel->ComputeSensitivity(Values); // TODO here

        if ( std::abs(std::abs(f(0)) - std::abs(f.segment<8>(1).array().abs().sum())) < tol ||
            Cursor->GetCurrentLevel() >= maxlevel ) {
    	    // stop subdividing, save result
    	    for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
    	        Cursor->ToChild(child);
    	        leafdata.push_back( std::abs(f(child+1)) / Cubes->GetVolume(child+1) );
    	        // TODO fval is just a test
    	        //leafdata.push_back( fval );
    	        leafids.push_back(Cursor->GetLeafId());
    	        KernelSum += f(child+1);
    	        Cursor->ToParent();
            }
    	    return;
        }

        for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
            Cursor->ToChild(child);
            VectorXr nv = (Values.array()-Values(0));
            if ( nv.norm() > 1e-2) { // TODO don't hard code this
                EvaluateKids( fval , Impl);
            } else {
                EvaluateKidsStatic( Values[child+1] );
            }
            Cursor->ToParent();
        }
        tcurse->Delete();
    }

    void OctreeGrid::EvaluateKidsStatic(const Real& fval) {

        assert("Evaluate Kids pre" && Cursor->CurrentIsLeaf());
        vtkHyperOctreeCursor *tcurse = Cursor->Clone();

        Real p[3];

        // at this level
        GetPosition(p);
        cpos <<  p[0], p[1], p[2];
        step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
        Cubes->SetLocation(0, cpos);
        Cubes->SetLength(0, step);
        Octree->SubdivideLeaf(Cursor);
        tcurse->ToSameNode(Cursor);

        std::cout << "\rStatic Leaf count: " << Octree->GetNumberOfLeaves();

        for (int child=0; child<Cursor->GetNumberOfChildren(); ++child) {
            Cursor->ToChild(child);
            assert(Cursor->CurrentIsLeaf());

            // Build cube
            GetPosition(p);
            cpos <<  p[0], p[1], p[2];
            step  = ((Size).array() / std::pow(2.,Cursor->GetCurrentLevel()));
            Cubes->SetLocation(child+1, cpos);
            Cubes->SetLength(child+1, step);

            Cursor->ToSameNode(tcurse);
        }

        // make calculation
        Cubes->ClearFields();
        // TODO if Values don't change much, we can set them static and use old logic
        VectorXcr f = SenseKernel->ComputeSensitivity(fval); // TODO here

        if ( std::abs(std::abs(f(0)) - std::abs(f.segment<8>(1).array().abs().sum())) < tol ||
            Cursor->GetCurrentLevel() >= maxlevel ) {
    	    // stop subdividing, save result
    	    for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
    	        Cursor->ToChild(child);
    	        leafdata.push_back( std::abs(f(child+1)) / Cubes->GetVolume(child+1) );
    	        //leafdata.push_back( 1 );
    	        // TODO fval is just a test
    	        //leafdata.push_back( fval );
    	        leafids.push_back(Cursor->GetLeafId());
    	        KernelSum += f(child+1);
    	        Cursor->ToParent();
            }
    	    return;
        }

        for (int child=0; child < Cursor->GetNumberOfChildren(); ++ child) {
            Cursor->ToChild(child);
            EvaluateKidsStatic( fval );
            Cursor->ToParent();
        }
        tcurse->Delete();
    }

    // ====================  ACCESS        =======================

    void OctreeGrid::SetLeafDataFromGridCreation() {

        if ( (int)(leafdata.size()) != Octree->GetNumberOfLeaves()  ) {
            std::cerr << "BADNESS IN GRID CREATIONS" << std::endl;
            std::cerr << "octreegrid.cpp:SetLeafDataFromGridCreation" << std::endl;
            exit(EXIT_FAILURE);
        }

        this->KernelArray->SetName("unnormalised kernel");
        this->KernelArray->SetNumberOfTuples(leafdata.size());

        //Real maxval = *max_element(leafdata.begin(), leafdata.end());
        for (unsigned int i=0; i<leafdata.size(); ++i) {
        //    KernelArray->SetValue(leafids[i], leafdata[i]/maxval);
            KernelArray->SetValue(leafids[i], leafdata[i]); //maxval);
        }

        //this->KernelArray->SetArray(&leafdata[0], leafdata.size(), 0);
        //this->Octree->GetLeafData()->AddArray( KernelArray );
        this->Octree->GetLeafData()->SetScalars( KernelArray );
    }

    void OctreeGrid::SetSize(const Vector3r& size) {
            this->Size = size;
            Octree->SetSize(size[0], size[1], size[2]);
    }

    void OctreeGrid::SetOrigin(const Vector3r& origin) {
            this->Origin = origin;
            Octree->SetOrigin(origin[0], origin[1], origin[2]);
    }

    void OctreeGrid::SetSize(const Real&x, const Real &y, const Real &z) {
            this->Size << x,y,z;
            Octree->SetSize(x, y, z);
    }

    void OctreeGrid::SetMinimumDepth(const Real& depth) {
        //Octree->SetOrigin(0,0,depth);
        //Origin << 0,0,depth;
        Octree->SetOrigin(Origin[0], Origin[1], depth);
        Origin(2) = depth;
    }

    vtkHyperOctree* OctreeGrid::GetVtkHyperOctree() {
        //this->Octree->Update();
        return this->Octree;
    }

    void OctreeGrid::SetKernel(Kernel *kern) {

        if (SenseKernel != NULL) {
            SenseKernel->DetachFrom(this);
        }

        kern->AttachTo(this);
        SenseKernel = kern;

        // TODO bug here, detach fail
        SenseKernel->SetFieldCubes(Cubes);

    }

    void OctreeGrid::SetLayeredEarth(LayeredEarth* model1d) {
        if (Model1D != NULL) {
            Model1D->DetachFrom(this);
        }
        model1d->AttachTo(this);
        Model1D = model1d;
    }


    // ====================  OPERATIONS    =======================

}       // -----  end of Lemma  name  -----

#endif //  LEMMAUSEVTK
