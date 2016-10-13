*$ CREATE MGDRAW.FOR
*COPY MGDRAW
*                                                                      *
*=== mgdraw ===========================================================*
*                                                                      *
      SUBROUTINE MGDRAW ( ICODE, MREG )

      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 1990-2006      by        Alfredo Ferrari           *
*     All Rights Reserved.                                             *
*                                                                      *
*                                                                      *
*     MaGnetic field trajectory DRAWing: actually this entry manages   *
*                                        all trajectory dumping for    *
*                                        drawing                       *
*                                                                      *
*     Created on   01 march 1990   by        Alfredo Ferrari           *
*                                              INFN - Milan            *
*     Last change  05-may-06       by        Alfredo Ferrari           *
*                                              INFN - Milan            *
*                                                                      *
*----------------------------------------------------------------------*
*
      INCLUDE '(CASLIM)'
      INCLUDE '(COMPUT)'
      INCLUDE '(SOURCM)'
      INCLUDE '(FHEAVY)'
      INCLUDE '(FLKSTK)'
      INCLUDE '(GENSTK)'
      INCLUDE '(MGDDCM)'
      INCLUDE '(PAPROP)'
      INCLUDE '(QUEMGD)'
      INCLUDE '(SUMCOU)'
      INCLUDE '(TRACKR)'
      INCLUDE '(LTCLCM)'
c
      include 'mgdraw.inc'
*
      DIMENSION DTQUEN ( MXTRCK, MAXQMG )
      integer  ICPART, IBPART 
      character*8 NEWREGNAM,MREGNAM
      CHARACTER*8 MLATNAM
      logical ldead
      double precision erawstartc, equenchedstartc
      double precision erawBM, equenchedBM
      double precision erawDC, equenchedDC
      double precision erawvtx, equenchedvtx
      double precision erawIT, equenchedIT
      double precision erawscint, equenchedscint
      double precision erawcry, equenchedcry
*
*----------------------------------------------------------------------*
*                                                                      *
*     Icode = 1: call from Kaskad                                      *
*     Icode = 2: call from Emfsco                                      *
*     Icode = 3: call from Kasneu                                      *
*     Icode = 4: call from Kashea                                      *
*     Icode = 5: call from Kasoph                                      *
*                                                                      *
*----------------------------------------------------------------------*
*                                                                      *
c      write(*,*)"sono in mgdraw"
*  +-------------------------------------------------------------------*
      if(idbflg.gt.1) then
         call GEOR2N ( mreg, MREGNAM, IERR ) 
         write(*,*)' '
         write(*,*)'------------- mgdraw: Ev =',ncase,' -------------'
         write(*,*)'jtrack = ',jtrack,' regione = ',MREGNAM
         write(*,*)' '
      endif
c
c************* PARTICLES AND HEAVY IONS WITH JTRACK=-2
      IF((JTRACK.GE.1).OR.((JTRACK.LE.-2).AND.(JTRACK.GE.-6)))THEN
         ICPART = ICHRGE(JTRACK)
         IBPART = IBARCH(JTRACK)
         AMPART = AM(JTRACK)
c*************HEAVY IONS WITH JTRACK.LT.-6
      ELSEIF(JTRACK.LT.-6)THEN
         ICPART = ICHEAV(-JTRACK)
         IBPART = IBHEAV(-JTRACK)
         AMPART = AMNHEA(-JTRACK)
      ELSE
         ICPART=int(zfrttk)
C         IBPART=0
C         ICPART = ICHRGE(JTRACK)
         IBPART = IBARCH(JTRACK)
         AMPART = AM(JTRACK)
      ENDIF
c
c**************************************************************
      CALL GEOR2N(MREG,MREGNAM,IERR)
      CALL GEON2R(MREGNAM,MREG,IERR)
      CALL GEOL2N(MLATTC,MLATNAM,IRTLAT,IERR)
c      write(*,*) jtrack, mreg,' ',MREGNAM, MLATTC
c
      if(idbflg.gt.1) then
         write(*,*)'ptrack = ',ptrack,' ekin = ',etrack-ampart
         write(*,*)'zfrttk = ',zfrttk,' icpart = ',icpart,'  m =',ampart
         WRITE (*,*)'x,y,z = ', ( SNGL (XTRACK (I)), SNGL (YTRACK (I)),
     &        SNGL (ZTRACK (I)), I = 0, NTRACK )
         WRITE (*,*)'cx,cy,cz = ',SNGL(CXTRCK),SNGL(CYTRCK),SNGL(CZTRCK)
         WRITE (*,*)'px,py,pz = ',SNGL(ptrack*CXTRCK),
     &        SNGL(ptrack*CYTRCK),SNGL(ptrack*CZTRCK)
         write(*,*)'time = ',atrack,' step = ',ctrack,' path = ',cmtrck
      endif
c
c Find which particle of the part common we are following:
c let's assume it's the last one...
c
*      if ((etrack-ampart).le.ZERZER) then
*         idead(idcurr) = 17
*         if (idbflg.gt.1) then
*            write(*,*) ' particle at end of range!  =>',
*     &           idcurr,nump,jpa(idcurr)
*         endif
*      endif

      call UpdateCurrPart(mreg,icpart,ibpart,ampart,icode,
     &     xtrack(0),ytrack(0),ztrack(0),JRUN)
c      call UpdateCurrPart(mreg,icpart,ibpart,ampart,icode)
c
            
c  *****************************************************************   
c  inside the start counter
c
      if( mreg.eq.nregstartc )then
         erawstartc = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawstartc = erawstartc + dtrack(ii)
            end do
            
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nello startc
c     
               do ii = 1,mtrack
                  equenchedstartc = equenchedstartc + dtquen(ii,3)
               end do
               equenchedstartc = equenchedstartc*abs_startc
            endif
         endif
         if(erawstartc.gt.0) then
            call score_startc(mreg,erawstartc,equenchedstartc,
     &      xtrack(0),ytrack(0),ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &      ztrack(ntrack))
         endif
      endif

c  *****************************************************************   
c  inside the beam monitor
c
      if( (mreg.ge.nregFirstU1.and.mreg.le.nregLastU1).or.
     &     (mreg.ge.nregFirstV1.and.mreg.le.nregLastV1) )then
         erawBM = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawBM = erawBM + dtrack(ii)
            end do
            
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nella camera a drift
c     
               do ii = 1,mtrack
                  equenchedBM = equenchedBM + dtquen(ii,3)
               end do
               equenchedBM = equenchedBM*abs_BM
            endif
         endif
         if(erawBM.gt.0) then
            call score_BM(mreg,erawBM,equenchedBM,
     &      xtrack(0),ytrack(0),ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &      ztrack(ntrack))
         endif
      endif

c     *************************************************************
c     inside the vertex
c
      if( mreg.le.nreglastvtx .and. mreg.ge.nregfirstvtx )then
         erawvtx = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawvtx = erawvtx + dtrack(ii)
            end do
            
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nel vertice
c     
               do ii = 1,mtrack
                  equenchedvtx = equenchedvtx + dtquen(ii,3)
               end do
               equenchedvtx = equenchedvtx*abs_vtx
            endif
         endif
         if(erawvtx.gt.0) then
            call score_vtx(mreg,erawvtx,equenchedvtx,
     &      xtrack(0),ytrack(0),ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &      ztrack(ntrack))
         endif
      endif

c     *************************************************************
c     inside the inner tracker
c
      if( mreg.le.nreglastIT .and. mreg.ge.nregfirstIT )then
         erawIT = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawIT = erawIT + dtrack(ii)
            end do
            
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nel vertice
c     
               do ii = 1,mtrack
                  equenchedIT = equenchedIT + dtquen(ii,3)
               end do
               equenchedIT = equenchedIT*abs_IT
            endif
         endif
         if(erawIT.gt.0) then
            call score_IT(mreg,erawIT,equenchedIT,
     &      xtrack(0),ytrack(0),ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &      ztrack(ntrack))
         endif
      endif
      
c  *****************************************************************   
c  inside the drift chamber
c
      if( (mreg.ge.nregFirstU2.and.mreg.le.nregLastU2).or.
     &     (mreg.ge.nregFirstV2.and.mreg.le.nregLastV2) )then
         erawDC = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawDC = erawDC + dtrack(ii)
            end do
            
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nella camera a drift
c     
               do ii = 1,mtrack
                  equenchedDC = equenchedDC + dtquen(ii,3)
               end do
               equenchedDC = equenchedDC*abs_DC
            endif
         endif
         if(erawDC.gt.0) then
            call score_DC(mreg,erawDC,equenchedDC,
     &      xtrack(0),ytrack(0),ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &      ztrack(ntrack))
         endif
      endif
c      
c     **********************************************************
c     Inside the scintillator
c
      if( mreg.le.nreglastscint .and. mreg.ge.nregfirstscint ) then
c         equenchedscint= 0.
         erawscint = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawscint = erawscint + dtrack(ii)
            end do
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato nello scintillatore
c     
               do ii = 1,mtrack
                  equenchedscint = equenchedscint + dtquen(ii,3)
               end do
               equenchedscint = equenchedscint*abs_plastic
            endif

         END IF
         if(erawscint.gt.0) then
            call score_scint(mreg,erawscint,equenchedscint,xtrack(0),
     &           ytrack(0),
     &           ztrack(0),xtrack(ntrack),ytrack(ntrack),ztrack(ntrack))
         endif
      endif
      
c     *************************************************************
c     inside the calorimeter
c      
      if( mreg.le.nreglastcry .and. mreg.ge.nregfirstcry )then
         erawcry = 0.
         IF ( MTRACK .GT. 0 )THEN
            do ii = 1,MTRACK
               erawcry = erawcry + dtrack(ii)
            end do
            IF ( LQEMGD )THEN
               RULLL  = ZERZER
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
c     
c     DTQUEN(MTRACK,1) e' il rilascio di energia quenchato
c     
               do ii = 1,mtrack
                  equenchedcry = equenchedcry + dtquen(ii,3)
               end do
               equenchedcry = equenchedcry*abs_cry
            endif
         endif
         if(erawcry.gt.0) then
            call score_cry(mreg,erawcry,equenchedcry,xtrack(0),
     &           ytrack(0), ztrack(0),xtrack(ntrack),ytrack(ntrack),
     &           ztrack(ntrack))
         endif
      endif

*     +----------------------------------------------------------
c     
c      write(*,*)'Esco da Mgdraw'
      RETURN
*     
*================================================================*
*                                                                      *
*     Boundary-(X)crossing DRAWing:                                    *
*                                                                      *
*     Icode = 1x: call from Kaskad                                     *
*             19: boundary crossing                                    *
*     Icode = 2x: call from Emfsco                                     *
*             29: boundary crossing                                    *
*     Icode = 3x: call from Kasneu                                     *
*             39: boundary crossing                                    *
*     Icode = 4x: call from Kashea                                     *
*             49: boundary crossing                                    *
*     Icode = 5x: call from Kasoph                                     *
*             59: boundary crossing                                    *
*                                                                      *
*======================================================================*
*                                                                      *
      ENTRY BXDRAW ( ICODE, MREG, NEWREG, XSCO, YSCO, ZSCO )
c     scoring at the boundaries... select the region
c     
c         write(*,*)'Sono in Bxdraw'
c************* PARTICLES AND HEAVY IONS WITH JTRACK=-2
       IF((JTRACK.GE.1).OR.((JTRACK.LE.-2).AND.(JTRACK.GE.-6)))THEN
         ICPART = ICHRGE(JTRACK)
         IBPART = IBARCH(JTRACK)
         AMPART = AM(JTRACK)
c************* HEAVY IONS WITH JTRACK.LT.-6
       ELSEIF(JTRACK.LT.-6)THEN
         ICPART = ICHEAV(-JTRACK)
         IBPART = IBHEAV(-JTRACK)
         AMPART = AMNHEA(-JTRACK)
       ELSE
         ICPART= ICHRGE(JTRACK)
         ICPART = ICHRGE(JTRACK)
         IBPART = IBARCH(JTRACK)
         AMPART = AM(JTRACK)
       ENDIF
c
c debug printing
c
      if(idbflg.gt.0) then
         call GEOR2N ( newreg, NEWREGNAM, IERR ) 
         call GEOR2N ( mreg, MREGNAM, IERR ) 
         CALL GEOL2N ( MLATTC,MLATNAM,IRTLAT,IERR )
         write(*,*)' '
         write(*,*)'------------- bxdraw: Ev =',ncase,' -------------'
         write(*,*)'jtrack = ',jtrack,' icode = ',icode
         write(*,*)'da -> ',mregnam,'a -> = ',newregnam, ' lat_idx = ',
     &        MLATTC

         if(idbflg.gt.0) then
            write(*,*)'zfrttk = ',zfrttk,' icpart = ',icpart,
     &           ' m =',AMPART
            write(*,*)'ptrack = ',ptrack,' ekin = ',etrack-AMPART
            write(*,*)'x,y,z = ',XSCO,YSCO,ZSCO
            WRITE (*,*)'cx,cy,cz = ',SNGL(CXTRCK), SNGL(CYTRCK)
     &           ,SNGL(CZTRCK)
            write(*,*)'number of track segments: ntrack = ',NTRACK
            do ii =1,ntrack
               write(*,*)'track seg # ',ii,' track seg lenght = '
     &              ,ttrack(ii)
            end do
            write(*,*)'number of en deposition: mtrack = ',MTRACK
            do ii =1,mtrack
               write(*,*)'track dep # ',ii,' track dep val = ',
     &              dtrack(ii)
            end do
         endif
         write(*,*)' '
      endif
cf
cf      
cf      CALL GEOL2N ( MLATTC,MLATNAM,IRTLAT,IERR )      
cf
c
c scoring the crossing info.
c
      if(JTRACK.ne.-1) then
         call score_cross(
     &        ICPART,IBPART,AMPART,newreg,mreg,xsco,ysco,zsco)
      endif
c      write(*,*)'Esco da Bxdraw'
c     
      RETURN
*     
* ======================================================================*
*                                                                      *
*     Event End DRAWing:                                               *
*                                                                      *
*======================================================================*
*                                                                      *
      ENTRY EEDRAW ( ICODE )
      if(idbflg.gt.2) then
         call dump_common()
      endif
      RETURN
*
*======================================================================*
*                                                                      *
*     ENergy deposition DRAWing:                                       *
*                                                                      *
*     Icode = 1x: call from Kaskad                                     *
*             10: elastic interaction recoil                           *
*             11: inelastic interaction recoil                         *
*             12: stopping particle                                    *
*             13: pseudo-neutron deposition                            *
*             14: escape                                               *
*             15: time kill                                            *
*     Icode = 2x: call from Emfsco                                     *
*             20: local energy deposition (i.e. photoelectric)         *
*             21: below threshold, iarg=1                              *
*             22: below threshold, iarg=2                              *
*             23: escape                                               *
*             24: time kill                                            *
*     Icode = 3x: call from Kasneu                                     *
*             30: target recoil                                        *
*             31: below threshold                                      *
*             32: escape                                               *
*             33: time kill                                            *
*     Icode = 4x: call from Kashea                                     *
*             40: escape                                               *
*             41: time kill                                            *
*             42: delta ray stack overflow                             *
*     Icode = 5x: call from Kasoph                                     *
*             50: optical photon absorption                            *
*             51: escape                                               *
*             52: time kill                                            *
*                                                                      *
*======================================================================*
*                                                                      *
      ENTRY ENDRAW ( ICODE, MREG, RULL, XSCO, YSCO, ZSCO )
*  +-------------------------------------------------------------------*
c
c debug
c
      if(idbflg.gt.1) then
         write(*,*)'------------- endraw: Ev =',ncase,' -------------'
         write(*,*)'jtrack = ',jtrack,' mreg = ',mreg
         write(*,*)'rull = ',rull,' icode = ',icode,' idcurr = ',idcurr
         write(*,*)'idead(idcurr) = ',idead(idcurr)
         write(*,*)'x,y,x = ',sngl(XSCO), sngl(YSCO), sngl(ZSCO) 
         WRITE (*,*)'cx,cy,cz = ',SNGL(CXTRCK),SNGL(CYTRCK),SNGL(CZTRCK)
      endif
c
Cgb
Cgb   to preserve idead flag correctly
Cgb
      idead (idcurr) = icode
Cgb
      if(idbflg.gt.1) then
         write(*,*)'ENDRAW: Now idead(idcurr) = ',idead(idcurr)
      endif
Cgb
      if(jtrack.lt.62) then
c************* PARTICLES AND HEAVY IONS WITH JTRACK=-2
         IF((JTRACK.GE.1).OR.((JTRACK.LE.-2).AND.(JTRACK.GE.-6)))THEN
            ICPART = ICHRGE(JTRACK)
            IBPART = IBARCH(JTRACK)
            AMPART = AM(JTRACK)
c*************HEAVY IONS WITH JTRACK.LT.-6
         ELSEIF(JTRACK.LT.-6)THEN
            ICPART = ICHEAV(-JTRACK)
            IBPART = IBHEAV(-JTRACK)
            AMPART = AMNHEA(-JTRACK)
         ELSE
            ICPART= ICHRGE(JTRACK)
            IBPART = IBARCH(JTRACK)
            AMPART = AM(JTRACK)
         ENDIF
c
c     if not delta ray below threshold than update the current particle
c
         if( (JTRACK.ne.3).or.(ICODE.ne.22)) then
c            write(*,*)'***********************************************'
c            write(*,*)' NB UpdateCurrentParticle Called from ENDRAW!!!'
c            write(*,*)'***********************************************'
            call UpdateCurrPart(mreg,icpart,ibpart,ampart,icode,
     &           xsco,ysco,zsco,JEND)
            if (jtrack.eq.7) then
               px(idcurr) = pxf(idcurr)
               py(idcurr) = pyf(idcurr)
               pz(idcurr) = pzf(idcurr)
            endif
         endif
      endif 
      if (rull.ne.0) then
c     
c  inside the start counter
c
         if( mreg.eq.nregstartc ) then
            erawstartc = rull
            equenchedstartc=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedstartc = dtquen(1,1)*abs_startc
            END IF
            call score_startc(mreg,erawstartc,equenchedstartc,xsco,
     &           ysco,zsco,xsco,ysco,zsco)
         endif

c     
c  inside the first drift chamber
c
         if( (mreg.ge.nregFirstU1.and.mreg.le.nregLastU1).or.
     &        (mreg.ge.nregFirstV1.and.mreg.le.nregLastV1) ) then
            erawBM = rull
            equenchedBM=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedBM = dtquen(1,1)*abs_BM
            END IF
            call score_BM(mreg,erawBM,equenchedBM,xsco,
     &           ysco,zsco,xsco,ysco,zsco)
         endif
c     
c  inside the vertex
c
         if( mreg.le.nreglastvtx .and. mreg.ge.nregfirstvtx )
     $        then
            erawvtx = rull
            equenchedvtx=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedvtx = dtquen(1,1)*abs_vtx
            END IF
            call score_vtx(mreg,erawvtx,equenchedvtx,xsco,
     &           ysco,zsco,xsco,ysco,zsco)
         endif
c     
c  inside the inner tracker
c
         if( mreg.le.nreglastIT .and. mreg.ge.nregfirstIT )
     $        then
            erawIT = rull
            equenchedIT=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedIT = dtquen(1,1)*abs_IT
            END IF
            call score_IT(mreg,erawIT,equenchedIT,xsco,
     &           ysco,zsco,xsco,ysco,zsco)
         endif
c     
c  inside the second drift chamber
c
         if( (mreg.ge.nregFirstU2 .and. mreg.le.nregLastU2).or.
     &        (mreg.ge.nregFirstV2 .and. mreg.le.nregLastV2) ) then
            erawDC = rull
            equenchedDC=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedDC = dtquen(1,1)*abs_DC
            END IF
            call score_DC(mreg,erawDC,equenchedDC,xsco,
     &           ysco,zsco,xsco,ysco,zsco)
         endif
c     
c  inside the Scint
c
         if ( mreg.le.nreglastscint .and. mreg.ge.nregfirstscint ) then
            erawscint = rull
            equenchedscint=0
            IF ( LQEMGD) THEN
               RULLL = RULL
               CALL QUENMG ( ICODE, MREG, RULL, DTQUEN )
               equenchedscint = dtquen(1,1)*abs_scint
            END IF
            call score_scint(mreg,erawscint,equenchedscint,xsco,ysco,
     &           zsco,xsco,ysco,zsco)
         endif
c
c     inside the Calorimeter
c
         if( mreg.le.nreglastcry .and. mreg.ge.nregfirstcry )then
            erawcry = rull
            equenchedcry = 0
            IF ( LQEMGD )THEN
               RULLL  = RULL
               CALL QUENMG ( ICODE, MREG, RULLL, DTQUEN )
               equenchedcry = dtquen(1,1)*abs_cry
            END IF
            call score_cry(mreg,erawcry,equenchedcry,xsco,ysco,
     &           zsco,xsco,ysco,zsco)
         endif
      endif
c
c     end debug
c     
      RETURN
*
*======================================================================*
*                                                                      *
*     SOurce particle DRAWing:                                         *
*                                                                      *
*======================================================================*
*
      ENTRY SODRAW
c
      if(idbflg.gt.1) then
         write(*,*)'------------- sodraw: Ev =',ncase,' -------------'
      endif
*  +-------------------------------------------------------------------*
*  |  (Radioactive) isotope: it works only for 1 source particle on
*  |  the stack for the time being
      IF ( ILOFLK (NPFLKA) .GE. 100000 .AND. LRADDC (NPFLKA) ) THEN
         IARES  = MOD ( ILOFLK (NPFLKA), 100000  )  / 100
         IZRES  = MOD ( ILOFLK (NPFLKA), 10000000 ) / 100000
         IISRES = ILOFLK (NPFLKA) / 10000000
         IONID  = ILOFLK (NPFLKA)
c
*  |
*  +-------------------------------------------------------------------*
*  |  Patch for heavy ions: it works only for 1 source particle on
*  |  the stack for the time being
      ELSE IF ( ABS (ILOFLK (NPFLKA)) .GE. 10000 ) THEN
         IONID = ILOFLK (NPFLKA)
         CALL DCDION ( IONID )
      ELSE 
         IONID = ILOFLK (NPFLKA)
      END IF
c
c   first call to initialing the kinematic tracking 
c 
      call UpdateCurrPart(mreg,icpart,ibpart,ampart,icode,
     &     0.d+00,0.d+00,0.d+00,JSTART)
c
      if(idbflg.gt.0) then
         write(*,*)'reg = ',numreg(nump),' tprod = ',tempo(nump),
     &        ' jpa = ',jpa(nump)
         write(*,*)'vert = ',vxi(nump),vyi(nump),vzi(nump),
     &        ' p = ',px(nump),py(nump),pz(nump)
      endif
*  |
*  +-------------------------------------------------------------------*
      RETURN
*
*======================================================================*
*                                                                      *
*     USer dependent DRAWing:                                          *
*                                                                      *
*     Icode = 10x: call from Kaskad                                    *
*             100: elastic   interaction secondaries                   *
*             101: inelastic interaction secondaries                   *
*             102: particle decay  secondaries                         *
*             103: delta ray  generation secondaries                   *
*             104: pair production secondaries                         *
*             105: bremsstrahlung  secondaries                         *
*             110: decay products                                      *
*     Icode = 20x: call from Emfsco                                    *
*             208: bremsstrahlung secondaries                          *
*             210: Moller secondaries                                  *
*             212: Bhabha secondaries                                  *
*             214: in-flight annihilation secondaries                  *
*             215: annihilation at rest   secondaries                  *
*             217: pair production        secondaries                  *
*             219: Compton scattering     secondaries                  *
*             221: photoelectric          secondaries                  *
*             225: Rayleigh scattering    secondaries                  *
*     Icode = 30x: call from Kasneu                                    *
*             300: interaction secondaries                             *
*     Icode = 40x: call from Kashea                                    *
*             400: delta ray  generation secondaries                   *
*  For all interactions secondaries are put on GENSTK common (kp=1,np) *
*  but for KASHEA delta ray generation where only the secondary elec-  *
*  tron is present and stacked on FLKSTK common for kp=npflka          *
*                                                                      *
*======================================================================*
*
      ENTRY USDRAW ( ICODE, MREG, XSCO, YSCO, ZSCO )
Cgb
Cgb     To mark correctly interaction code of current particle
Cgb
      intcode(idcurr) = icode
c     
c debug printing
c
      if(idbflg.gt.0) then
         call GEOR2N ( mreg, MREGNAM, IERR ) 
         write(*,*)' '
         write(*,*)'------------- usdraw: Ev =',ncase,' -------------'
         write(*,*)' Fluka index= ',ISPUSR(MKBMX2),' idcurr= ',idcurr,
     &        'jtrack = ',jtrack,' icode = ',icode
         write(*,*)'regione = ',mregnam, 'x,y,z = ',XSCO,YSCO,ZSCO
         if(idbflg.gt.2) then
            write(*,*)'zfrttk = ',zfrttk,' icpart = ',icpart,',m = ',
     &           AMPART
            write(*,*)'ptrack = ',ptrack,' ekin = ',etrack-AMPART,
     &           ' t = ',atrack
            WRITE (*,*)'cx,cy,cz = ',SNGL(CXTRCK), SNGL(CYTRCK)
     &           ,SNGL(CZTRCK)
         endif
         write(*,*)'  USDRAW secondary writing : NP=',NP
         do  ip = 1, NP
            WRITE(*, *) ip,' jpa sec= ',KPART(ip),' Ek sec= ',
     &           TKI(ip),' p sec= ',plr(ip)
         end do
         write(*,*)'  USDRAW heavy secondary writing : NPHEAV=',NPHEAV
         do  ip = 1, NPHEAV
            WRITE(*, *) ip,' jpa sec= ',KHEAVY(ip),' Ek sec= ',
     &           tkheav(ip),' p sec= ',pheavy(ip)
         end do
         write(*,*)' '
      endif

      if(idbflg.gt.0) then
         write(*,*)'TGreg= ',nregtarg,' MREG= ',MREG,
     &        ' ICODE= ',ICODE
      endif
      
      IF (MREG.eq.nregtarg .AND. ICODE.EQ.101) THEN
         tarfrag = 1
         if (np.gt.2) tarfrag = 2
         if (npheav.gt.0) tarfrag = 3
         if (idbflg.gt.2) then
            do  ip = 1, NP
               if(kpart(ip).lt.-1) then
                  CALL USRDCI(kpart(ip),IONA,IONZ,IONM)
                  write(*,*)'A= ',iona,' Z= ',ionz
               endif
            end do
         endif
      ENDIF
*
*   Do nothing for elastic scattering
c      if ( icode .eq.  100) return
*  +-------------------------------------------------------------------*
*  |  Delta rays, Compton and Moller-Bhabba scattering:
      IF ( ICODE.EQ.103 .OR. ICODE.EQ.210 .OR. ICODE.EQ. 212 ) THEN
         ldead = .false.
*  |
*  +-------------------------------------------------------------------*
*  |  Bremsstrahlung:
      ELSE IF ( ICODE .EQ. 208 .OR. ICODE .EQ. 105 ) THEN
         NPA    = NP
         ldead = .false.
*  +-------------------------------------------------------------------*
*  |  Compton scattering:
      ELSE IF ( ICODE .EQ. 219 ) THEN
         idead (idcurr) = icode
         ldead = .true.
*  +-------------------------------------------------------------------*
*  |  PhotoElectric:
      ELSE IF ( ICODE .EQ. 221 ) THEN
         idead (idcurr) = icode
         ldead = .true.
*  |
*  +-------------------------------------------------------------------*
*  |  In flight or at rest annihilation:
      ELSE IF ( ICODE .EQ. 214 .OR. ICODE .EQ. 215 ) THEN
         idead (idcurr) = icode
         ldead = .true.
*  |
*  +-------------------------------------------------------------------*
*  |  Pair production:
      ELSE IF ( ICODE.EQ.217 .OR. ICODE.EQ.104 ) THEN
         idead (idcurr) = icode
         ldead = .true.
*  |
*  +-------------------------------------------------------------------*
*  |  Low energy neutron secondaries:
      ELSE IF ( ICODE.EQ.300 ) THEN
         idead (idcurr) = icode
         ldead =.true.
*  |
*  +-------------------------------------------------------------------*
*  |  Decays
      ELSE IF ( ICODE.EQ.102 .OR. icode.eq.110) THEN
         idead (idcurr) = icode
         ldead = .true.
*  |
*  +-------------------------------------------------------------------*
*  |  Inelastic interactions:
      ELSE IF ( ICODE.EQ.101 ) THEN
         idead (idcurr) = icode
         ldead = .true.
*  |
*  +-------------------------------------------------------------------*
*  |  Everything else:
      ELSE
         ldead = .false.
      END IF
*  |
*  +-------------------------------------------------------------------*
      numint = numint +1
      xint(numint) = xsco
      yint(numint) = ysco
      zint(numint) = zsco
      intpa(numint) = idcurr
C      idead(numint) = idead(idcurr)
      if(idbflg.gt.1) then
         write(*,*)'USDRAW: Now numint, idead(numint) = ',
     &        numint,intpa(numint)
      endif
      intcode(numint) = intcode(idcurr)
Cgb
Cgb      idead (numint) = intcode(idcurr)
Cgb
      IF (icode.eq.219 .or. icode.eq.101 .or. icode.eq.102 .or. 
     &     icode.eq.110 .or. icode.eq.214 .or. icode.eq.215 .or.
     &     icode.eq.217 .or. icode.eq.221 ) then
Cgb
         call UpdateCurrPart(mreg,0,0,zerzer,icode,
     &        xsco,ysco,zsco,JEND)
Cgb
      endif
c
      if ( ldead ) then
         idead (idcurr) = icode
c         idead (idcurr) = intcode(idcurr)
         if(idbflg.gt.1) then
            write(*,*)'USDRAW: ldead Now idcurr, idead(idcurr) = ',
     &           idcurr,idead(idcurr),icode
         endif
         vxf (idcurr) = sngl(xsco)
         vyf (idcurr) = sngl(ysco)
         vzf (idcurr) = sngl(zsco)
      end if
c
      RETURN
*=== End of subrutine Mgdraw ==========================================*
      END

