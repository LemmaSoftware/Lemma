"highlight cComment ctermfg=Green guifg=Green
highlight cType ctermfg=Green guifg=Green
syn keyword cType Real Complex VectorXr VectorXcr Vector3r Vector3Xr VectorXi Vector3cr Vector3Xcr MatrixXr MatrixXi MatrixXcr shared_ptr make_shared

highlight constType ctermfg=Red guifg=Red
syn keyword constType  PI EPSILON0 MU0 QPI  

highlight eType ctermfg=Magenta guifg=Magenta
syn keyword eType  MAGUNITS  TEMPUNITS  TIMEUNITS FREQUENCYUNITS FEMCOILORIENTATION ORIENTATION FIELDTYPE FIELDCOMPONENT SPATIALCOORDINANT HANKELTRANSFORMTYPE FIELDCALCULATIONS WINDOWTYPE 

highlight eeType ctermfg=Cyan guifg=Cyan
syn keyword eeType  TESLA NANOTESLA GAUSS CELCIUS KELVIN SEC MILLISEC MICROSEC NANOSEC PICOSEC HZ KHZ MHZ GHZ COAXIAL COPLANAR HFIELDREAL HFIELDIMAG EFIELDREAL EFIELDIMAG XCOMPONENT YCOMPONENT ZCOMPONENT XCOORD YCOORD ZCOORD X Y Z NX  NY  NZ ANDERSON801 CHAVE FHTKEY201 FHTKEY101 FHTKEY51 QWEKEY E H BOTH HAMMING HANNING RECTANGULAR

" Namespaces
highlight nspace ctermfg=Red guifg=Red
syn keyword nspace Lemma YAML Eigen 

" Lemma types
highlight leType ctermfg=Yellow guifg=Yellow
syn keyword leType HankelTransform FHTAnderson801 FHTKey201 FHTKey101 FHTKey51 GQChave QWEKey KernelEM1D KernelEM1DSpec KernelEM1DBase KernelEM1DManager DipoleSource EarthModel LayeredEarth LayeredEarthEM TEMSurvey TEMSurveyLine TEMSurveyLineRecord TEMInductiveReceiver WireAntenna PolygonalWireAntenna TEMTransmitter TEMReceiver Instrument InstrumentTem LemmaObject FieldPoints DCIPElectrode TEMSurveyData TEMSurveyLineData TEMSurveyLineRecordData  ASCIIParser CubicSplineInterpolator RectilinearGrid GridReader RectilinearGridReader RectilinearGridVTKExporter Filter WindowFilter DEMParticle DEMGrain Data DataReader EMEarth1D 

" Deprecated Lemma Types
highlight dleType ctermfg=Blue guifg=Blue 
syn keyword dleType  ReferenceCountedObject 
