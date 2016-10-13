COPY USREIN
*
*=== Usrein ===========================================================*
*
      SUBROUTINE USREIN

      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(CASLIM)'
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 1991-2005      by    Alfredo Ferrari & Paola Sala  *
*     All Rights Reserved.                                             *
*                                                                      *
*                                                                      *
*     USeR Event INitialization: this routine is called before the     *
*     showering of an event is started, but after the source particles *
*     of that event have been already loaded on the stack              *
*                                                                      *
*     Created on 01 january 1991   by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*     Last change on 09-apr-99     by    Alfredo Ferrari               *
*                                                                      *
*                                                                      *
*----------------------------------------------------------------------*
*
c
      include 'mgdraw.inc'
      integer ii  
c
      if(idbflg .gt. 0)then
         write(*,*)'***************************************************'
         write(*,*)'                   EVENT',ncase
         write(*,*)'***************************************************'
         write(*,*)''
      endif
c
      numev = ncase
      tarfrag = 0
c
      do ii = 1,min(nump,maxnump)
         idpa(ii)   = 0
         igen(ii)   = 0
         icha(ii)   = 0
         numreg(ii) = 0
         iba(ii)    = 0
         idead(ii)  = 0
         jpa(ii)    = 0
         vxi(ii)    = 0.
         vyi(ii)    = 0.
         vzi(ii)    = 0.
         vxf(ii)    = 0.
         vyf(ii)    = 0. 
         vzf(ii)    = 0.
         px(ii)     = 0.
         py(ii)     = 0.
         pz(ii)     = 0.
         pxf(ii)    = 0.
         pyf(ii)    = 0.
         pzf(ii)    = 0.
         amass(ii)  = 0.
         tempo(ii)  = 0.
         tof(ii)    = 0.
         trlen(ii)  = 0.
c
         idfluka(ii) = 0   ! aux variables for particle latching
c
      end do
      nump = 0
c
c
c
      do ii = 1,min(numint,maxint)
         xint(ii)    = 0
         yint(ii)    = 0
         zint(ii)    = 0
         intpa(ii)   = 0
         intcode(ii) = 0
      end do
      numint = 0
c
c     start counter
c
      nstartc = 0
      do ii = 1,maxstartc
         idstartc(ii)    = 0
         xinstartc(ii)   = 0.  
         xoutstartc(ii)  = 0. 
         yinstartc(ii)   = 0. 
         youtstartc(ii)  = 0. 
         zinstartc(ii)   = 0.
         zoutstartc(ii)  = 0.
         pxinstartc(ii)  = 0.  
         pxoutstartc(ii) = 0. 
         pyinstartc(ii)  = 0. 
         pyoutstartc(ii) = 0. 
         pzinstartc(ii)  = 0.
         pzoutstartc(ii) = 0.
         destartc(ii)    = 0.
         alstartc(ii)    = 0.
         timstartc(ii)   = 0.
      end do
c
c     first drift chamber
c
      nBM = 0
      do ii = 1,maxBM
         idBM(ii)     = 0
         iviewBM(ii)  = 0
         iplaBM(ii) = 0
         icellBM(ii)  = 0
         xinBM(ii)    = 0.  
         xoutBM(ii)   = 0. 
         yinBM(ii)    = 0. 
         youtBM(ii)   = 0. 
         zinBM(ii)    = 0.
         zoutBM(ii)   = 0.
         pxinBM(ii)   = 0.  
         pxoutBM(ii)  = 0. 
         pyinBM(ii)   = 0. 
         pyoutBM(ii)  = 0. 
         pzinBM(ii)   = 0.
         pzoutBM(ii)  = 0.
         deBM(ii)     = 0.
         alBM(ii)     = 0.
         timBM(ii)    = 0.
      end do
c     
c     vertex 
c
      nvtx = 0
      do ii = 1,maxvtx
         idvtx(ii)    = 0
         iplavtx(ii)  = 0
         irowvtx(ii)  = 0
         icolvtx(ii)  = 0
         xinvtx(ii)   = 0.  
         xoutvtx(ii)  = 0. 
         yinvtx(ii)   = 0. 
         youtvtx(ii)  = 0. 
         zinvtx(ii)   = 0.
         zoutvtx(ii)  = 0.
         pxinvtx(ii)  = 0.  
         pxoutvtx(ii) = 0. 
         pyinvtx(ii)  = 0. 
         pyoutvtx(ii) = 0. 
         pzinvtx(ii)  = 0.
         pzoutvtx(ii) = 0.
         devtx(ii)    = 0.
         alvtx(ii)    = 0.
         timvtx(ii)   = 0.
      end do
c     
c     inner tracker 
c
      nIT = 0
      do ii = 1,maxIT
         idIT(ii)    = 0
         iplaIT(ii)  = 0
         irowIT(ii)  = 0
         icolIT(ii)  = 0
         xinIT(ii)   = 0.  
         xoutIT(ii)  = 0. 
         yinIT(ii)   = 0. 
         youtIT(ii)  = 0. 
         zinIT(ii)   = 0.
         zoutIT(ii)  = 0.
         pxinIT(ii)  = 0.  
         pxoutIT(ii) = 0. 
         pyinIT(ii)  = 0. 
         pyoutIT(ii) = 0. 
         pzinIT(ii)  = 0.
         pzoutIT(ii) = 0.
         deIT(ii)    = 0.
         alIT(ii)    = 0.
         timIT(ii)   = 0.
      end do
c     
c     second drift chamber
c
      nDC = 0
      do ii = 1,maxDC
         idDC(ii)     = 0
         iviewDC(ii)  = 0
         iplaDC(ii) = 0
         icellDC(ii)  = 0
         xinDC(ii)    = 0.  
         xoutDC(ii)   = 0. 
         yinDC(ii)    = 0. 
         youtDC(ii)   = 0. 
         zinDC(ii)    = 0.
         zoutDC(ii)   = 0.
         pxinDC(ii)   = 0.  
         pxoutDC(ii)  = 0. 
         pyinDC(ii)   = 0. 
         pyoutDC(ii)  = 0. 
         pzinDC(ii)   = 0.
         pzoutDC(ii)  = 0.
         deDC(ii)     = 0.
         alDC(ii)     = 0.
         timDC(ii)    = 0.
      end do
c
c     scintillator
c      
      nscint = 0
      do ii= 1, maxscint
         idscint(ii)    = 0
         irowscint(ii)  = 0
         icolscint(ii)  = 0
         xinscint(ii)   = 0.
         yinscint(ii)   = 0.
         zinscint(ii)   = 0.
         pxinscint(ii)  = 0.
         pyinscint(ii)  = 0.
         pzinscint(ii)  = 0.
         timscint(ii)   = 0.
         xoutscint(ii)  = 0. 
         youtscint(ii)  = 0.
         zoutscint(ii)  = 0.
         pxoutscint(ii) = 0.
         pyoutscint(ii) = 0.
         pzoutscint(ii) = 0.
         descint(ii)    = 0.
         alscint(ii)    = 0.
      enddo
c
c     calorimeter
c      
      ncry = 0
      do ii= 1, maxcry
         idcry(ii)    = 0
         irowcry(ii)  = 0
         icolcry(ii)  = 0
         xincry(ii)   = 0.
         yincry(ii)   = 0.
         zincry(ii)   = 0.
         pxincry(ii)  = 0.
         pyincry(ii)  = 0.
         pzincry(ii)  = 0.
         timcry(ii)   = 0.
         xoutcry(ii)  = 0. 
         youtcry(ii)  = 0.
         zoutcry(ii)  = 0.
         pxoutcry(ii) = 0.
         pyoutcry(ii) = 0.
         pzoutcry(ii) = 0.
         decry(ii)    = 0.
         alcry(ii)    = 0.
      enddo
c
c     crossings
c      
      ncross = 0
      do ii= 1, nmaxcross
         idcross(ii)       = 0
         nregcross(ii)     = 0
         nregoldcross(ii)  = 0
         nlattcross(ii)    = 0
         nlattoldcross(ii) = 0
         pxcross(ii)       = 0.
         pycross(ii)       = 0.
         pzcross(ii)       = 0.
         xcross(ii)        = 0.
         ycross(ii)        = 0.
         zcross(ii)        = 0.
         tcross(ii)        = 0.
         chcross(ii)       = 0.
         amacross(ii)      = 0.
      end do
      ncross = 0
c
c     root event initialization
c
c     CALL myusrein(NCASE)
c

      RETURN
*=== End of subroutine Usrein =========================================*
      END


