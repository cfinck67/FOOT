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
      do ii =1,ncry
         Eqcalo = Eqcalo + decry(ii)
      end do
c
      do ii =1,nscint
         Eqscint = Eqscint + descint(ii)
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
         write(outunit,*) ncase,nump,nstartc,nBM,nvtx,nIT,nDC,nscint,
     &        ncry,ncross
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
         do ii = 1,nstartc   
            write(outunit,*) idstartc(ii), 
     &           xinstartc(ii), yinstartc(ii), zinstartc(ii),
     &           xoutstartc(ii), youtstartc(ii), zoutstartc(ii),
     &           pxinstartc(ii), pyinstartc(ii), pzinstartc(ii), 
     &           pxoutstartc(ii), pyoutstartc(ii), pzoutstartc(ii),
     &           destartc(ii), alstartc(ii), timstartc(ii)
         end do
c
c     scrivo la prima camera a drift
c         
         do ii = 1,nBM   
            write(outunit,*) idBM(ii), iviewBM(ii), iplaBM(ii), 
     &           icellBM(ii), xinBM(ii), yinBM(ii), zinBM(ii),
     &           xoutBM(ii), youtBM(ii), zoutBM(ii),
     &           pxinBM(ii), pyinBM(ii), pzinBM(ii), 
     &           pxoutBM(ii), pyoutBM(ii), pzoutBM(ii),
     &           deBM(ii), alBM(ii), timBM(ii)
         end do
c     
c     scrivo il vertice
c         
         do ii = 1,nvtx
            write(outunit,*) idvtx(ii), iplavtx(ii), irowvtx(ii),
     &           icolvtx(ii), xinvtx(ii), yinvtx(ii), zinvtx(ii),
     &           xoutvtx(ii), youtvtx(ii), zoutvtx(ii),
     &           pxinvtx(ii), pyinvtx(ii), pzinvtx(ii), 
     &           pxoutvtx(ii), pyoutvtx(ii), pzoutvtx(ii),
     &           devtx(ii), alvtx(ii), timvtx(ii)
         end do
c     
c     scrivo l'inner tracker
c         
         do ii = 1,nIT
            write(outunit,*) idIT(ii), iplaIT(ii), irowIT(ii),
     &           icolIT(ii), xinIT(ii), yinIT(ii), zinIT(ii),
     &           xoutIT(ii), youtIT(ii), zoutIT(ii),
     &           pxinIT(ii), pyinIT(ii), pzinIT(ii), 
     &           pxoutIT(ii), pyoutIT(ii), pzoutIT(ii),
     &           deIT(ii), alIT(ii), timIT(ii)
         end do
c
c     scrivo la seconda camera a drift
c         
         do ii = 1,nDC   
            write(outunit,*) idDC(ii), iviewDC(ii),
     &           iplaDC(ii), icellDC(ii), 
     &           xinDC(ii), yinDC(ii), zinDC(ii),
     &           xoutDC(ii), youtDC(ii), zoutDC(ii),
     &           pxinDC(ii), pyinDC(ii), pzinDC(ii), 
     &           pxoutDC(ii), pyoutDC(ii), pzoutDC(ii),
     &           deDC(ii), alDC(ii), timDC(ii)
         end do
c
c     scrivo lo scintillatore
c     
         do ii = 1,nscint
            write(outunit,*) idscint(ii),
     &           irowscint(ii), icolscint(ii),
     &           xinscint(ii), yinscint(ii), zinscint(ii),
     &           xoutscint(ii), youtscint(ii), zoutscint(ii),
     &           pxinscint(ii), pyinscint(ii), pzinscint(ii), 
     &           pxoutscint(ii), pyoutscint(ii), pzoutscint(ii),
     &           descint(ii), alscint(ii), timscint(ii)
         end do
c     
c     scrivo il calorimetro
c         
         do ii = 1,ncry
            write(outunit,*) idcry(ii), irowcry(ii), icolcry(ii), 
     &           xincry(ii), yincry(ii), zincry(ii),
     &           xoutcry(ii), youtcry(ii), zoutcry(ii),
     &           pxincry(ii), pyincry(ii), pzincry(ii), 
     &           pxoutcry(ii), pyoutcry(ii), pzoutcry(ii),
     &           decry(ii), alcry(ii), timcry(ii)
         end do
c
c     scrivo i crossings
c
         do ii = 1,ncross 
            write(outunit,*) idcross(ii), nregcross(ii),
     &           nregoldcross(ii), xcross(ii), ycross(ii), zcross(ii),
     &           pxcross(ii), pycross(ii), pzcross(ii),
     &           amacross(ii), chcross(ii), tcross(ii)
         end do
c
c     
c     call myusreou()
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
c     write(*,*)'esco da  usreou'
c
      RETURN
*===  End of subroutine Usreou =========================================*
      END





