c     
c     idcurr = position of the current particle in the particle common block
c     if idcurr<=0  -> new particle to be followed     
c     
      integer idbflg, outunit
      integer tarfrag, fragtrig
      integer numev
      integer idcurr
      integer JSTART, JRUN, JEND
      parameter (JSTART=1, JRUN=0, JEND=-1)
      double precision Ethrdep
      common /comunico/Ethrdep, idbflg, numev,
     &     idcurr, outunit, tarfrag, fragtrig
c     
c     particle common block
c     
c     nump          = number of tracks
c     
c     for each of the produced track we register:
c     
c     idpa          = index in the part common of the track parent
c     icha          = charge
c     iba           = barionic number
c     idead         = with which interaction the particle dies
c     jpa           = kind of particle
c     igen          = kind of interaction
c     vxi, vyi, vzi = production position of the track
c     vxf, vyf, vzf = final position of the track
c     px,py,pz,     = production momentum of the track
c     amass         = track mass
c     tempo         = production time of the track
c     tof           = tof of the particle
c     trlen         = track lenght of the particle
c     
      integer nump, maxnump
      parameter(maxnump=1000)

      integer idpa(maxnump), igen(maxnump)
      integer icha(maxnump), numreg(maxnump), iba(maxnump)
      integer idead(maxnump), jpa(maxnump)
      real vxi(maxnump),vyi(maxnump),vzi(maxnump)
      real vxf(maxnump),vyf(maxnump),vzf(maxnump)
      real px(maxnump),py(maxnump),pz(maxnump)
      real pxf(maxnump),pyf(maxnump),pzf(maxnump)
      real amass(maxnump), tempo(maxnump), tof(maxnump)
      real trlen(maxnump)
      common /particle_common/ vxi, vyi, vzi, 
     &     vxf, vyf, vzf,px, py, pz, pxf, pyf, pzf,
     &     amass, tempo, tof, trlen, nump, idpa, igen, 
     &     icha, numreg, iba, idead, jpa

c     *************************************************************
c     common needed to score the hit in the start counter
c     
      integer nstartc, maxstartc 
      parameter (maxstartc=2000)
      integer idstartc (maxstartc)

      real xinstartc(maxstartc), xoutstartc(maxstartc), 
     &     yinstartc(maxstartc), youtstartc(maxstartc), 
     &     zinstartc(maxstartc), zoutstartc(maxstartc)
      real pxinstartc(maxstartc), pxoutstartc(maxstartc), 
     &     pyinstartc(maxstartc), pyoutstartc(maxstartc), 
     &     pzinstartc(maxstartc), pzoutstartc(maxstartc)
      real destartc(maxstartc), timstartc(maxstartc), 
     &     alstartc(maxstartc)
      common /startc_common/ xinstartc, xoutstartc, yinstartc, 
     &     youtstartc, zinstartc, zoutstartc, pxinstartc, pxoutstartc,
     &     pyinstartc, pyoutstartc, pzinstartc, pzoutstartc,
     &     destartc, alstartc, timstartc, nstartc, idstartc

c     *************************************************************
c     common needed to score the hit in the beam monitor
c     
c     iview = x and y view(1-2)
c     ipla  = first drift chamber plane index (1-6)
c     icell = first drift chamber cell index (1-3)
c      
      integer nBM, maxBM 
      parameter (maxBM=2000)
      integer idBM (maxBM), iviewBM(maxBM),iplaBM(maxBM),
     &     icellBM(maxBM) 

      real xinBM(maxBM), xoutBM(maxBM), yinBM(maxBM), 
     &     youtBM(maxBM), zinBM(maxBM), zoutBM(maxBM)
      real pxinBM(maxBM), pxoutBM(maxBM), pyinBM(maxBM), 
     &     pyoutBM(maxBM), pzinBM(maxBM), pzoutBM(maxBM)
      real deBM(maxBM), timBM(maxBM), alBM(maxBM)
      common /BM_common/ xinBM, xoutBM, yinBM, youtBM, zinBM, 
     &     zoutBM, pxinBM, pxoutBM, pyinBM, pyoutBM, pzinBM, 
     &     pzoutBM,deBM, alBM, timBM, nBM, idBM, iviewBM,
     &     iplaBM, icellBM
c     
c  common for the indexing of the first drift ch
c
      integer MAXBMREG
      parameter (MAXBMREG=500)
      integer ireg2plaBM(MAXBMREG), ireg2viewBM(MAXBMREG), 
     & ireg2cellBM(MAXBMREG)
      common /BM_index_common/ireg2plaBM, ireg2viewBM, ireg2cellBM

c     *************************************************************
c     common needed to score the hit in the vertex tracker
c     
      double precision xminvtx, yminvtx, dxvtx, dyvtx
      integer xpixvtx, ypixvtx
      parameter (xminvtx=-1.)
      parameter (yminvtx=-1.)
      parameter (dxvtx=0.002)
      parameter (dyvtx=0.002)
      parameter (xpixvtx=1000)
      parameter (ypixvtx=1000)

c     iplavtx  =  vertex plane (1-3) 
c     icolvtx  =  vertex pixel column index (1-1000)
c     irowvtx  =  vertex pixel row index (1-1000)

      integer nvtx, maxvtx 
      parameter (maxvtx=2000)
      integer idvtx (maxvtx), iplavtx(maxvtx), irowvtx(maxvtx), 
     &     icolvtx(maxvtx) 

      real xinvtx(maxvtx), xoutvtx(maxvtx), yinvtx(maxvtx),
     &     youtvtx(maxvtx), zinvtx(maxvtx), zoutvtx(maxvtx)
      real pxinvtx(maxvtx), pxoutvtx(maxvtx), pyinvtx(maxvtx), 
     &     pyoutvtx(maxvtx), pzinvtx(maxvtx), pzoutvtx(maxvtx)
      real devtx(maxvtx), timvtx(maxvtx), alvtx(maxvtx)
      common /vtx_common/ xinvtx, xoutvtx, yinvtx, youtvtx, 
     &     zinvtx, zoutvtx, pxinvtx, pxoutvtx, pyinvtx, pyoutvtx,
     &     pzinvtx, pzoutvtx, devtx, alvtx, timvtx, nvtx, idvtx,
     &     iplavtx, irowvtx, icolvtx

c     *************************************************************
c     common needed to score the hit in the inner tracker
c     
      double precision xminIT, yminIT, dxIT, dyIT
      integer xpixIT, ypixIT
      parameter (xminIT=-3.)
      parameter (yminIT=-3.)
      parameter (dxIT=dxvtx)
      parameter (dyIT=dyvtx)
      parameter (xpixIT=3000)
      parameter (ypixIT=3000)

c     iplaIT  =  inner tracker plane (1-2) 
c     icolIT  =  inner tracker pixel column index (1-3000)
c     irowIT  =  inner tracker pixel row index (1-3000)

      integer nIT, maxIT 
      parameter (maxIT=2000)
      integer idIT (maxIT), iplaIT(maxIT), irowIT(maxIT), 
     &     icolIT(maxIT) 

      real xinIT(maxIT), xoutIT(maxIT), yinIT(maxIT),
     &     youtIT(maxIT), zinIT(maxIT), zoutIT(maxIT)
      real pxinIT(maxIT), pxoutIT(maxIT), pyinIT(maxIT), 
     &     pyoutIT(maxIT), pzinIT(maxIT), pzoutIT(maxIT)
      real deIT(maxIT), timIT(maxIT), alIT(maxIT)
      common /IT_common/ xinIT, xoutIT, yinIT, youtIT, 
     &     zinIT, zoutIT, pxinIT, pxoutIT, pyinIT, pyoutIT,
     &     pzinIT, pzoutIT, deIT, alIT, timIT, nIT, idIT,
     &     iplaIT, irowIT, icolIT

c     *************************************************************
c     common needed to score the hit in the drift chamber
c
c     iview = x and y view (1-2)
c     ipla  = second drift chamber plane index (1-6)
c     icell = second drift chamber cell index (1-6)
c
      integer nDC, maxDC 
      parameter (maxDC=2000)
      integer idDC (maxDC), iviewDC(maxDC),iplaDC(maxDC),
     &     icellDC(maxDC) 

      real xinDC(maxDC), xoutDC(maxDC), yinDC(maxDC), 
     &     youtDC(maxDC), zinDC(maxDC), zoutDC(maxDC)
      real pxinDC(maxDC), pxoutDC(maxDC),p yinDC(maxDC), 
     &     pyoutDC(maxDC), pzinDC(maxDC), pzoutDC(maxDC)
      real deDC(maxDC), timDC(maxDC), alDC(maxDC)
      common /DC_common/ xinDC, xoutDC, yinDC, youtDC, zinDC, 
     &     zoutDC, pxinDC, pxoutDC, pyinDC, pyoutDC, pzinDC, 
     &     pzoutDC,deDC, alDC, timDC, nDC, idDC, iviewDC,
     &     iplaDC, icellDC
c     
c  common for the indexing of the second drift ch
c
      integer MAXDCREG
      parameter (MAXDCREG=500)
      integer ireg2plaDC(MAXDCREG), ireg2viewDC(MAXDCREG), 
     & ireg2cellDC(MAXDC)
      common /DC_index_common/ireg2plaDC, ireg2viewDC, ireg2cellDC

c      
c     *************************************************************
c     common needed to score the hit in the scintillator
c
c     irowscint = scintillator row index (1-11)
c     icolscint = scintillator col index (1-11)
c      
      integer nscint, maxscint
      parameter (maxscint=1000)
      integer idscint(maxscint)
      integer irowscint(maxscint), icolscint(maxscint)
      real xinscint(maxscint), xoutscint(maxscint), yinscint(maxscint),
     &     youtscint(maxscint), zinscint(maxscint), zoutscint(maxscint)
      real pxinscint(maxscint), pxoutscint(maxscint),
     &     pyinscint(maxscint), pyoutscint(maxscint),
     &     pzinscint(maxscint), pzoutscint(maxscint)
      real descint(maxscint),alscint(maxscint), timscint(maxscint)
      common /scint_common/ xinscint, 
     &     xoutscint, yinscint, youtscint, zinscint, zoutscint, 
     &     pxinscint, pxoutscint, pyinscint, pyoutscint, pzinscint, 
     &     pzoutscint, descint, alscint, timscint, nscint, idscint,
     &     irowscint, icolscint
c     
c  common for the indexing of the scint
c
      integer MAXSCINTREG
      parameter (MAXSCINTREG=500)
      integer ireg2rowscint(MAXSCINTREG), ireg2colscint(MAXSCINTREG)
      common /scint_index_common/ireg2rowscint, ireg2colscint
      
c     *************************************************************
c     common neede to score the hit in the Calo regions
c     
c     irowcry    =  crystal row index (1-11)
c     icolcry    =  crystal column index (1-11)
      
      integer ncry, maxcry
      parameter (maxcry=2000)
      integer idcry(maxcry), irowcry(maxcry), icolcry(maxcry)

      real xincry(maxcry), xoutcry(maxcry), yincry(maxcry),
     &     youtcry(maxcry), zincry(maxcry), zoutcry(maxcry)
      real pxincry(maxcry), pxoutcry(maxcry), pyincry(maxcry), 
     &     pyoutcry(maxcry), pzincry(maxcry), pzoutcry(maxcry)
      real decry(maxcry), timcry(maxcry), alcry(maxcry)
      common /cry_common/ xincry, xoutcry, yincry, youtcry, zincry, 
     &     zoutcry, pxincry, pxoutcry, pyincry, pyoutcry, pzincry, 
     &     pzoutcry, decry, alcry, timcry, ncry, idcry, irowcry, icolcry
c     
c  common for the indexing of the scint
c
      integer MAXCRYREG
      parameter (MAXCRYREG=500)
      integer ireg2rowcry(MAXCRYREG), ireg2colcry(MAXCRYREG)
      common /cry_index_common/ireg2rowcry, ireg2colcry
      
c     *************************************************************
c     common needed to score the boundary crossing info 
c     and put the right pointer to the particle common of the produced electrons
c     
      integer ncross, maxcross
      parameter (maxcross=10000)
      integer idcross(maxcross), nregcross(maxcross) 
      integer nregoldcross(maxcross), nlattcross(maxcross)
      integer nlattoldcross(maxcross)
      real pxcross(maxcross), pycross(maxcross), pzcross(maxcross)
      real xcross(maxcross), ycross(maxcross), zcross(maxcross)
      real tcross(maxcross), amacross(maxcross), chcross(maxcross)
      common /crssing_common/ 
     &     pxcross, pycross, pzcross, 
     &     xcross, ycross, zcross, tcross, chcross, amacross, ncross,  
     &     idcross, nregcross, nregoldcross, nlattcross, nlattoldcross
      
c     ************************************************************
c     Numbers of fluka region to be scored
c     
      integer nregaria, nregfirstcry, nreglastcry,
     &     nregfirstvtx, nreglastvtx, nregfirstIT, nreglastIT, 
     &     nregFirstU1, nreglastU1, nregFirstV1, nregLastV1, 
     &     nregFirstU2, nregLastU2, nregFirstV2, nregLastV2,
     &     nregstartc, nregtarg, nregfirstscint, nreglastscint
      
      common/region_common/
     &     nregaria, nregfirstcry, nreglastcry, 
     &     nregfirstvtx, nreglastvtx, nregfirstIT, nreglastIT,
     &     nregFirstU1, nreglastU1, nregFirstV1, nregLastV1, 
     &     nregFirstU2, nregLastU2, nregFirstV2, nregLastV2,
     &     nregstartc, nregtarg, nregfirstscint, nreglastscint  
      
c     ***********************************************************
c     absolute light efficiency of material in quenched formula
c     
      double precision abs_plastic, abs_cry,abs_vtx,abs_startc,
     &     abs_BM,abs_DC,abs_IT
      parameter (abs_plastic=1.0)
      parameter (abs_cry=1.)   
      parameter (abs_vtx=1.0)
      parameter (abs_IT=1.0)
      parameter (abs_startc=1.0)
      parameter (abs_BM=1.0)
      parameter (abs_DC=1.0)

c     ***********************************************************
c     interaction stack: common block for the parent particle
c     recording with the info on the interaction point
c     
      integer idfluka(maxnump)
      integer numint, maxint
      parameter (MAXINT=2000)
      double precision xint(maxint),yint(maxint),zint(maxint)
      integer intpa(maxint),intcode(maxint)
      common /particle_stack/xint,yint,zint,intpa,intcode,
     &     idfluka,numint
