*$ CREATE USRINI.FOR
*COPY USRINI
*
*=== usrini ===========================================================*
*
      SUBROUTINE USRINI ( WHAT, SDUM )

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
*     USeR INItialization: this routine is called every time the       *
*                          USRICALL card is found in the input stream  *
*                                                                      *
*                                                                      *
*     Created on 01 january 1991   by    Alfredo Ferrari & Paola Sala  *
*      Infn -     Milan                                                *
*                                                                      *
*     Last change on 20-mar-05     by    Alfredo Ferrari               *
*                                                                      *
*                                                                      *
*----------------------------------------------------------------------*
*
      DIMENSION WHAT (6)
      CHARACTER SDUM*8

c
      INCLUDE '(FLKMAT)'
      include "mgdraw.inc"
      character*8 REGNAM
      CHARACTER*8 LATNAM
      integer pladc,celldc
      integer colscint,rowscint
      integer colcry,rowcry
      integer nrowcry,ncolcry
      integer nrowscint,ncolscint
      integer nplavtx
      integer n1DCpla, n1DCcell
      integer n2DCpla, n2DCcell

      nrowcry = 11
      ncolcry = 11
      nrowscint = 11
      ncolscint = 11
      nplavtx = 5
      n1DCpla = 6
      n1DCcell = 3
      n2DCpla = 6
      n2DCcell = 6
*
*
c      write(*,*)"sono in usrini"
*  Don't change the following line:
      LUSRIN = .TRUE.
* *** Write from here on *** *
c
      idbflg = int(what(1))
      fragtrig = int(what(2))
      Ehtrdep = what(3)/1000.
c 
c  init of geometry parameters
c
c
      write(*,*)" "
      write(*,*)"fragtrig= ",fragtrig," Ethrdep= ",Ehtrdep
c
      outunit=60
      open(unit=outunit,file='TXT.dat',form='formatted',status='new')
c
c writing the header
c
      write(outunit,100) fragtrig, Ehtrdep
c
c find the region number of the region of interest
c
      nregtarg = 0
      nregaria = 0
      nregstartc = 0
      nregfirstvtx = 1000000
      nreglastvtx = 0
      nregfirstscint = 1000000
      nreglastscint = 0
      nregfirstcry = 1000000 
      nreglastcry = 0 
      nregFirstU1 = 1000000
      nregFirstV1 = 1000000
      nregLastU1 = 0
      nregLastV1 = 0
      nregFirstU2 = 1000000
      nregFirstV2 = 1000000
      nregLastU2 = 0
      nregLastV2 = 0

      ia = 1
      ib = 1
      ic = 1
      id = 1
      ie = 1
      ig = 1
      ih = 1
      ik = 1
      il = 1

      do nn = 1,MAX1DCREG
         ireg2view1DC(nn) = -1
         ireg2cell1DC(nn) = -1
         ireg2pla1DC(nn) = -1
      end do
      do nn = 1,MAX2DCREG
         ireg2view2DC(nn) = -1
         ireg2cell2DC(nn) = -1
         ireg2pla2DC(nn) = -1
      end do
      do nn = 1,MAXSCINTREG
         ireg2rowscint(nn) = -1
         ireg2colscint(nn) = -1
      end do
      do nn = 1,MAXCRYREG
         ireg2rowcry(nn) = -1
         ireg2colcry(nn) = -1
      end do
c
c
      do ii = 1,NREGS
         call GEOR2N ( ii, REGNAM, IERR )
c         write(*,3333)REGNAM
c 3333    format(1x,A8)
         if(ierr.eq.0) then
            if(REGNAM(1:6).eq.'TARGET') nregtarg = ii
            if(REGNAM(1:3).eq.'VTX') then
               read(REGNAM(4:4),*) plavtx
               if(ib.eq.1) then
                  nregfirstvtx=ii
               elseif(ib.eq.nplavtx) then
                  nreglastvtx=ii
               endif
               ib = ib + 1
            elseif(REGNAM(1:6).eq.'STARTC')then     
               nregstartc=ii
            elseif(REGNAM.eq.'AIR') then
               nregaria=ii
            elseif(REGNAM(1:4).eq.'SCIN') then
               read(REGNAM(5:6),*) rowscint
               read(REGNAM(7:8),*) colscint
               ireg2rowscint(ii) = rowscint
               ireg2colscint(ii) = colscint
               if(ic.eq.1) then
                  nregfirstscint=ii
               elseif(ic.eq.(nrowscint*ncolscint)) then
                  nreglastscint=ii
               endif
               ic = ic + 1
            elseif(REGNAM(1:4).eq.'CALO') then
               read(REGNAM(5:6),*) rowcry
               read(REGNAM(7:8),*) colcry
               ireg2rowcry(ii) = rowcry
               ireg2colcry(ii) = colcry
               if(id.eq.1) then
                  nregfirstcry=ii
               elseif(id.eq.(nrowcry*ncolcry)) then
                  nreglastcry=ii
               endif
               id = id + 1
            elseif(REGNAM(1:2).eq.'up') then
               read(REGNAM(3:3),*) pladc
               read(REGNAM(5:5),*) celldc
               if (REGNAM(6:6).eq.'a')then
c                  write(*,*)'cheppalleeeeeeeeeeeee'
                  ireg2view1DC(ii) = 1
c                  write(*,*)'ireg2view1DC  ',ireg2view1DC(ii)
                  ireg2pla1DC(ii) = pladc
                  ireg2cell1DC(ii) = celldc
                  if(ig.eq.1) then
                     nregFirstU1=ii
                  elseif(ig.eq.(n1DCpla*n1DCcell)) then
                     nregLastU1=ii
                  endif
                  ig = ig + 1
               elseif (REGNAM(6:6).eq.'b')then
                  ireg2view2DC(ii) = 1
                  ireg2pla2DC(ii) = pladc
                  ireg2cell2DC(ii) = celldc
                  if(ih.eq.1) then
                     nregFirstU2=ii
                  elseif(ih.eq.(n2DCpla*n2DCcell)) then
                     nregLastU2=ii
                  endif
                  ih = ih + 1
               endif
            elseif(REGNAM(1:2).eq.'vp') then
               read(REGNAM(3:3),*) pladc
               read(REGNAM(5:5),*) celldc
               if (REGNAM(6:6).eq.'a')then
                  ireg2view1DC(ii) = 2
                  ireg2pla1DC(ii) = pladc
                  ireg2cell1DC(ii) = celldc
                  if(ik.eq.1) then
                     nregFirstV1=ii
                  elseif(ik.eq.(n1DCpla*n1DCcell)) then
                     nregLastV1=ii
                  endif
                  ik = ik + 1
               elseif (REGNAM(6:6).eq.'b')then
                  ireg2view2DC(ii) = 2
                  ireg2pla2DC(ii) = pladc
                  ireg2cell2DC(ii) = celldc
                  if(il.eq.1) then
                     nregFirstV2=ii
                  elseif(il.eq.(n2DCpla*n2DCcell)) then
                     nregLastV2=ii
                  endif
                  il = il + 1
               endif
            endif
         endif
      end do

       

      write(*,*)'======================================'
      write(*,*)'USRINI: idbflg =  ',idbflg
c     
      if(((nregtarg*nreglastvtx*nregaria*nreglastscint*nregstartc
     &     *nreglastcry*nregLastU1*nregLastV1*nregLastU2*nregLastV2)
     &     .eq.0).or.(nregfirstvtx.eq.1000000).or.
     &     (nregfirstcry.eq.1000000).or.(nregfirstscint.eq.1000000).or.
     &     (nregFirstU1.eq.1000000).or.(nregFirstV1.eq.1000000).or.
     &     (nregFirstU2.eq.1000000).or.(nregFirstV2.eq.1000000))then
         write(*,*)'Non ho trovato tutte le regioni!!!!'
      else
         write(*,*)'**************** Inizio Geometria *****************'
         write(*,*)'        nregaria           = ',nregaria
         write(*,*)'        nregtarg           = ',nregtarg
         write(*,*)'        nregstartc         = ',nregstartc
         write(*,*)'        nregfirstvtx       = ',nregfirstvtx
         write(*,*)'        nreglastvtx        = ',nreglastvtx
         write(*,*)'        nregFirstV1        = ',nregFirstV1
         write(*,*)'        nregLastV1         = ',nregLastV1
         write(*,*)'        nregFirstU1        = ',nregFirstU1
         write(*,*)'        nregLastU1         = ',nregLastU1
         write(*,*)'        nregFirstV2        = ',nregFirstV2
         write(*,*)'        nregLastV2         = ',nregLastV2
         write(*,*)'        nregFirstU2        = ',nregFirstU2
         write(*,*)'        nregLastU2         = ',nregLastU2
         write(*,*)'        nregfirstscint     = ',nregfirstscint
         write(*,*)'        nreglastscint      = ',nreglastscint
         write(*,*)'        nregfirstcrystal   = ',nregfirstcry
         write(*,*)'        nreglastcrystal    = ',nreglastcry
         write(*,*)'**************** Fine Geometria *******************'
         write(*,*)''
      endif
      write(*,*)' '
      write(*,*)'======================================'
c
c init of the root interface
c
c      CALL myusrini();
c     
c      write(*,*)"esco da usrini"

 100  FORMAT(I2, 1X, F6.4, 3(1X, I2))
      RETURN
*===  End of subroutine Usrini ================================*

      END

