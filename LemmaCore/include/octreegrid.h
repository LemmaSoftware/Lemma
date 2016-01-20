/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/19/2010
  @version  $Id: octreegrid.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  OCTREEGRID_INC
#define  OCTREEGRID_INC

// This class depends on VTK!

#ifdef  LEMMAUSEVTK
#include "grid.h"
#include "vtkHyperOctree.h"        // In Filtering
#include "vtkHyperOctreeCursor.h"
#include "vtkHyperOctreeSampleFunction.h"
#include "vtkImplicitFunction.h"
#include "vtkCell.h"
#include "vtkGenericCell.h"

#ifdef  LEMMA_SINGLE_PRECISION
#include "vtkFloatArray.h"
#else      // -----  not LEMMA_SINGLE_PRECISION  -----
#include "vtkDoubleArray.h"
#endif     // -----  not LEMMA_SINGLE_PRECISION  -----

#include "kernel.h"
#include "receiverpoints.h"
#include "receivercubes.h"
#include "layeredearth.h"
//#include "layeredearthnmr.h"
#include "inversesolver.h"

#include "FastDelegate.h"
#include "FastDelegateBind.h"

namespace Lemma {

    // forward declaration
    class InverseSolver ;

    // ===================================================================
    //        Class:  OctreeGrid
    /** An Octree class, builds off VtkHyperOctree so this class can
        only be used when compiled with VTK libs.
        An Octree is a heirarchial 3D mesh with rectangular cells containing
        either 0 or 8 children.
     */
    // ===================================================================
    class OctreeGrid : public Grid {

        friend class InverseSolver;
        //friend class SNMRInversion1D;        // From Merlin Module

        friend std::ostream &operator<<(std::ostream &stream,
                const OctreeGrid &ob);

        public:

            // ====================  LIFECYCLE     =======================

            /**
             * new grid octreee
             */
            static OctreeGrid* New();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

            // ====================  OPERATORS     =======================


            // ====================  ACCESS        =======================

            /** Sets the kernel function to be used to make the mesh.
             *  @param[in] kern is a kernel that is used to generate a mesh.
             */
            void SetKernel(Kernel* kern);

            /// Sets a 1D model associated with this kernel.
            /// @param[in] model1d is a pointer to a LayeredEarth model that
            /// will be used to pre-subdivide leaves that overlap several
            /// layers.
            void SetLayeredEarth(LayeredEarth* model1d);

            /** Sets the size of the mesh
                @param[in] size is the size of the mesh. The mesh must start at
                [0,0,0] and goes to [sizex, sizey, sizez].
             */
            void SetSize(const Vector3r& size);

            /** Sets the origin of the mesh
                @param[in] origin is the origin of the mesh.
             */
            void SetOrigin(const Vector3r& origin);

            /** Sets the minimum depth to model. This must be greater than 0.
                @param[in] depth is the new minimum depth.
             */
            void SetMinimumDepth(const Real& depth);

            /** Fills in the leaf data with the kernel sensitivity
             */
            void FillMeshWithKernel();

            /** Fills in the leaf data with the kernel sensitivity
             */
            void FillMeshWithKernel(vtkImplicitFunction* Impl);

            /** Sets the size of the mesh
             */
            void SetSize(const Real &x, const Real &y, const Real &z);

            /** Generates mesh to specified tolerance.
             *  @param[in] tol is the kernel tolerance.
             */
            void GenerateMesh(const Real &tol);


            /** Generates a mesh to specified tolerance, but initially splits
             *  based on implicit function.
             *  @param[in] tol is the kernel tolerance.
             *  @param[in] Impl is the implicit dataset used for initial
             *  subdivision
             *  @param[in] impltol is the tolerance for the initial model based
             *  subdividing.
             */
            void GenerateMesh(const Real& tol, vtkImplicitFunction* Impl,
                              const Real& impltol);

            /** Generates an Octree mesh using a pre-sampled input Octree Dataset.
                Speeds up repeated kernel calculations when the underlying model in
                the above ImplicitFunction needs to be called often or is complex.
             */
            void GenerateMesh(const Real& tol, vtkImplicitFunction* Impl,
                    vtkHyperOctreeSampleFunction* OctSamp);

            /** Evaluates the kernel on the mesh. In some cases this is the same thing
                as generate mesh, but if more that one calculation are needed on a
                mesh you can use this method. It interfaces with the kernels by
                calling
             */
            Complex EvaluateKernelOnMesh();

            /** Returns a receiver cube class that represents the same Geometry as the
                HyperOctree. This is useful as it can be passed directly to em1dearth,
                and currently, an Octree cannot. But this requires storage of all the
                cell volumes and locations. In the future, you should be able to pass
                around this class just like a ReceiverCubes.
             */
            void ReceiverCubeRepresentation();

            /** Returns the receiver cube class. */
            ReceiverCubes* GetReceiverCubes();

            /** returns the kernel sum from generation */
            Complex GetKernelSum();

            /** Accessor to underlying vtkClass.
             *  @return a vtkHyperOctree that can be used for visualization.
             */
            vtkHyperOctree* GetVtkHyperOctree( );

            /** Traverse the data structure and call a function at each leaf
             */
            void TraverseAndCall(InverseSolver* Inverse);

            /** Traverse the data structure and call a function at each leaf
             */
            void TraverseAndCall( vtkImplicitFunction* Impl );

            /**
             */
            void TraverseLeaves(vtkImplicitFunction* Impl);

            /**
             */
            void FollowExistingTree(vtkHyperOctree* Leader, vtkHyperOctreeCursor* LeaderCursor,
                    vtkImplicitFunction* Impl);

            // ====================  OPERATIONS    =======================

            /** Binds any derived member function of a derived class of
                LayeredEarth to SampleScalarFromLayeredEarthFD. This function is then
                used to sample the Octree mesh for that parameter. The member function
                needs to be of the type GetLayerXXX(const int Layer& lay).
                @param[in] earth is a pointer to a derived class of LayeredEarth
                @param[in] fcnptr is a function pointer to a member function. It
                should be called like &LayeredEarthNMR::GetLayerPorosity
                @param[in] name is a std::string identifier.
             */
            template <class Derived>
            void SampleScalarFromLayeredEarth(LayeredEarth* earth,
                    Real (Derived::*fcnptr)(const int& id), const std::string &name) {

                SampleScalarFromLayeredEarthFD.bind(static_cast<Derived*>(earth), fcnptr);
                GetLayerInt.bind(earth, &LayeredEarth::GetLayerAtThisDepth);
                // TODO, don't use KernelArray, instead create new Array, and set it
                // in there. Then delete?
                this->KernelArray->SetName(name.c_str());
                this->KernelArray->SetNumberOfTuples(Octree->GetNumberOfLeaves());
                SampleScalar();
                this->Octree->GetLeafData()->SetScalars( KernelArray );
                this->Octree->GetLeafData()->AddArray( KernelArray );
            }

            /** Sends cursor to root
             */
            void CursorToRoot();

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor. */
            OctreeGrid (const std::string &name);

            /** Default protected constructor. */
            ~OctreeGrid ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  OPERATIONS    =======================

            /** Used internally. The leaf data is saved and then set as the vtk leaf
             *  data.
             */
            void SetLeafDataFromGridCreation();

            /** Return position of leaf, returns centre location
             */
            void GetPosition( Real* p );

            /** Used to fill leaf data with a scalar value from a layered earth
             *  model
             */
            fastdelegate::FastDelegate1< const int&, Real > SampleScalarFromLayeredEarthFD;

            /** Used to get the Layer of delagated model.
             */
            fastdelegate::FastDelegate1< const Real&, int >  GetLayerInt;

            /** Recursively determines the mesh to use
             */
            void EvaluateKids( );

            /** Recursively determines the mesh to use
                @param[in] kval is the current leaf kernel value
                           which may be reused in analysis.
             */
            void EvaluateKids( Complex kval );

            /** Same as above, but uses an implict function.
             */
            void EvaluateKids (vtkImplicitFunction* Impl);

            /** Same as above, but lets function value be passed directly
             */
            void EvaluateKids (const Real& fval, vtkImplicitFunction* Impl);

            /** Same as above but is passed a static fval to reuse. Useful when underlying model is smooth
             */
            void EvaluateKidsStatic(const Real& fval);

            /** Same as above, but lets function value be passed directly
             */
            void SampleFunction(vtkImplicitFunction* Impl);

            /** Recursively determines the mesh to use
             */
            void FillGrid( );

            /** Recursively determines the mesh to use
             */
            void FillGrid(vtkImplicitFunction* Impl);

            /** Used internally to build a receiver cube representation
             */
            void AssembleReceiverCubes( );

            /** Samples a scalar from a LayeredEarth object. Used internally and calls
             *  SampleScalarFromLayeredEarthFD, which can be set using
             *  SampleScalarFromLayeredEarth.
             */
            void SampleScalar();



            // Returns ReceiverPoints for the Octree.
            //void ComputeFieldOnOctree( WireAntenna* loop );

            // ====================  DATA MEMBERS  =========================

        private:

            Vector3r                Size;
            Vector3r                Origin;
            Vector3r                step;
            Vector3r                cpos;

            int                     level;
            int                     maxlevel;
            int                     index;
            int                     counter;

            Real                    cvol;
            Real                    tvol;
            Real                    tol;
            Complex                 KernelSum;

            Eigen::Matrix<Real, 8, 3> Posadd;

            std::vector<Real>       leafdata;
            std::vector<vtkIdType>  leafids;

            Kernel                 *SenseKernel;
            ReceiverCubes          *Cubes;
            LayeredEarth           *Model1D;

            vtkHyperOctree               *Octree;
            vtkHyperOctreeCursor         *Cursor;
            vtkHyperOctreeSampleFunction *OctSamp;

            #ifdef  LEMMA_SINGLE_PRECISION
            vtkFloatArray          *KernelArray;
            #else      // -----  not LEMMA_SINGLE_PRECISION  -----
            vtkDoubleArray         *KernelArray;
            #endif     // -----  not LEMMA_SINGLE_PRECISION  -----
            vtkDataArray           *SampleArray;

    }; // -----  end of class  OctreeGrid  -----

}       // -----  end of Lemma  name  -----

#endif     // -----  not LEMMAUSEVTK  -----
#endif   // ----- #ifndef OCTREEGRID_INC  -----
