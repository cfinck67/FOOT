*$ CREATE USREOU.FOR
*COPY USREOU
*
*=== Usreou ===========================================================*
*
      SUBROUTINE USREOU

      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
*
*----------------------------------------------------------------------*
*                                                                      *
*     Copyright (C) 1991-2005      by    Alfredo Ferrari & Paola Sala  *
*     All Rights Reserved.                                             *
*                                                                      *
*                                                                      *
*     USeR Event OUtput: this routine is called at the end of each     *
*     event                                                            *
*                                                                      *
*     Created on 01 january 1991   by    Alfredo Ferrari & Paola Sala  *
*                                                   Infn - Milan       *
*                                                                      *
*     Last change on 09-apr-99     by    Alfredo Ferrari               *
*                                                                      *
*                                                                      *
*----------------------------------------------------------------------*
*
      include '(CASLIM)'
      include 'mgdraw.inc'
      integer ii
      logical trigger
      double precision Eqcalo, Eqscint
c     
c     
      trigger = .false.
      Eqcalo = 0
      Eqscint = 0
c
      do ii =1,nCAL
         Eqcalo = Eqcalo + deCAL(ii)
      end do
c
      do ii =1,nSCN
         Eqscint = Eqscint + deSCN(ii)
      end do
c
      if( (Eqcalo.ge.Ethrdep) .or. (Eqscint.gt.Ethrdep) ) then
         trigger = .true.
      endif
      if (fragtrig.gt.0 .AND. tarfrag.le.0) then
         trigger = .false.
      endif
c     
      if(idbflg.gt.0) then
         write(*,*)'ev= ',numev,' Equench= ',Eqcalo,
     &        ' Eqscint= ',Eqscint,
     &        ' trigger= ', Trigger,
     &        ' Eqcalo= ',Eqcalo,' Eqscint= ',Eqscint,
     &        ' Ethrdep= ',Ethrdep,' tarfrag= ',tarfrag
      endif
c
c
      if(trigger) then
c
         write(outunit,*) ncase,nump,nSTC,nBMN,nVTX,nITR,nDCH,nSCN,nCAL,
     &        nCROSS
c
c     scrivo la banca delle particelle
c
         do ii = 1,nump
            write(outunit,*)idpa(ii), igen(ii), icha(ii), 
     &           numreg(ii), iba(ii), idead(ii), jpa(ii), vxi(ii), 
     &           vyi(ii), vzi(ii), vxf(ii), vyf(ii), vzf(ii), px(ii), 
     &           py(ii),pz(ii),pxf(ii),pyf(ii),pzf(ii),amass(ii), 
     &           tempo(ii),tof(ii),trlen(ii)
         end do
c
c     scrivo lo start counter
c         
         do ii = 1,nSTC   
            write(outunit,*) idSTC(ii), 
     &           xinSTC(ii), yinSTC(ii), zinSTC(ii),
     &           xoutSTC(ii), youtSTC(ii), zoutSTC(ii),
     &           pxinSTC(ii), pyinSTC(ii), pzinSTC(ii), 
     &           pxoutSTC(ii), pyoutSTC(ii), pzoutSTC(ii),
     &           deSTC(ii), alSTC(ii), timSTC(ii)
         end do
c
c     scrivo il beam monitor
c         
         do ii = 1,nBMN   
            write(outunit,*) idBMN(ii), iviewBMN(ii), ilayBMN(ii), 
     &           icellBMN(ii), xinBMN(ii), yinBMN(ii), zinBMN(ii),
     &           xoutBMN(ii), youtBMN(ii), zoutBMN(ii),
     &           pxinBMN(ii), pyinBMN(ii), pzinBMN(ii), 
     &           pxoutBMN(ii), pyoutBMN(ii), pzoutBMN(ii),
     &           deBMN(ii), alBMN(ii), timBMN(ii)
         end do
c     
c     scrivo il vertice
c         
         do ii = 1,nVTX
            write(outunit,*) idVTX(ii), ilayVTX(ii), irowVTX(ii),
     &           icolVTX(ii), xinVTX(ii), yinVTX(ii), zinVTX(ii),
     &           xoutVTX(ii), youtVTX(ii), zoutVTX(ii),
     &           pxinVTX(ii), pyinVTX(ii), pzinVTX(ii), 
     &           pxoutVTX(ii), pyoutVTX(ii), pzoutVTX(ii),
     &           deVTX(ii), alVTX(ii), timVTX(ii)
         end do
c     
c     scrivo l'inner tracker
c         
         do ii = 1,nITR
            write(outunit,*) idITR(ii), ilayITR(ii), irowITR(ii),
     &           icolITR(ii), xinITR(ii), yinITR(ii), zinITR(ii),
     &           xoutITR(ii), youtITR(ii), zoutITR(ii),
     &           pxinITR(ii), pyinITR(ii), pzinITR(ii), 
     &           pxoutITR(ii), pyoutITR(ii), pzoutITR(ii),
     &           deITR(ii), alITR(ii), timITR(ii)
         end do

c     
c     scrivo la seconda camera a drift
c         
         do ii = 1,nDCH   
            write(outunit,*) idDCH(ii), iviewDCH(ii),
     &           ilayDCH(ii), icellDCH(ii), 
     &           xinDCH(ii), yinDCH(ii), zinDCH(ii),
     &           xoutDCH(ii), youtDCH(ii), zoutDCH(ii),
     &           pxinDCH(ii), pyinDCH(ii), pzinDCH(ii), 
     &           pxoutDCH(ii), pyoutDCH(ii), pzoutDCH(ii),
     &           deDCH(ii), alDCH(ii), timDCH(ii)
         end do
c
c     scrivo lo scintillatore
c
         do ii = 1,nSCN
            write(outunit,*) idSCN(ii),
     &           istripSCN(ii), iviewSCN(ii),
     &           xinSCN(ii), yinSCN(ii), zinSCN(ii),
     &           xoutSCN(ii), youtSCN(ii), zoutSCN(ii),
     &           pxinSCN(ii), pyinSCN(ii), pzinSCN(ii), 
     &           pxoutSCN(ii), pyoutSCN(ii), pzoutSCN(ii),
     &           deSCN(ii), alSCN(ii), timSCN(ii)
         end do
c     
c     scrivo il calorimetro
c         
         do ii = 1,nCAL
            write(outunit,*) idCAL(ii), icryCAL(ii),
     &           xinCAL(ii), yinCAL(ii), zinCAL(ii),
     &           xoutCAL(ii), youtCAL(ii), zoutCAL(ii),
     &           pxinCAL(ii), pyinCAL(ii), pzinCAL(ii), 
     &           pxoutCAL(ii), pyoutCAL(ii), pzoutCAL(ii),
     &           deCAL(ii), alCAL(ii), timCAL(ii)
         end do
c
c     scrivo i crossings
c
         do ii = 1,nCROSS 
            write(outunit,*) idCROSS(ii), nregCROSS(ii),
     &           nregoldCROSS(ii), xCROSS(ii), yCROSS(ii), zCROSS(ii),
     &           pxCROSS(ii), pyCROSS(ii), pzCROSS(ii),
     &           amaCROSS(ii), chCROSS(ii), tCROSS(ii)
         end do
c
c     
c
      endif
c     
      if(idbflg .gt. 0)then
         call dump_common()
         write(*,*)'***************************************************'
         write(*,*)'***************** EVENT END ***********************'
         write(*,*)'***************************************************'
         write(*,*)''
      endif
c
c 
c
      RETURN
*===  End of subroutine Usreou =========================================*
      END





