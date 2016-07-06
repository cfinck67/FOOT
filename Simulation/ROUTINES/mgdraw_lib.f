c----------------------------------------------------
      subroutine dump_common_part() 
c-----------------------------------------------------
c
      integer ii
      include 'mgdraw.inc'
      double precision ptot
c
      write(*,*)' '
      write(*,*)'------- dump_common_part ---------'
      write(*,*)' '
c
      write(*,*)'DUMP common PART'
      write(*,*)'numero particelle = ',nump
      do ii = 1, nump
         write(*,*)'particle = ', ii
         write(*,*)'idparent = ', idpa(ii),' igen = ',igen(ii), 
     &        ' charge = ',icha(ii),' reg =',numreg(ii)
         write(*,*)'idead = ',idead(ii),' jpa =',jpa(ii),
     &        ' mass = ',amass(ii),' time = ', tempo(ii)
         write(*,*)'vert in = ',vxi(ii),vyi(ii),vzi(ii)
         write(*,*)'vert fin = ',vxf(ii),vyf(ii),vzf(ii)
         write(*,*)'p ini = ',px(ii),py(ii),pz(ii)
         ptot = sqrt(px(ii)**2+py(ii)**2+pz(ii)**2)
         write(*,*)'p tot ini = ',ptot,' ekin ini= ',sqrt(ptot*ptot +
     &        amass(ii)*amass(ii))-amass(ii)
         write(*,*)'p fin = ',pxf(ii),pyf(ii),pzf(ii)
         write(*,*)'p tot fin = ',sqrt(pxf(ii)**2+pyf(ii)**2+pzf(ii)**2)
      end do
      return 
      end
c
c      
c----------------------------------------------------
      subroutine dump_common_scint() 
c-----------------------------------------------------
c
      integer ii
      include 'mgdraw.inc'
c
      write(*,*)' '
      write(*,*)'------- dump_common_scint ---------'
      write(*,*)' '
c
      write(*,*)'numero rilasci in scint: ',nscint
      do ii = 1,nscint
         write(*,*)'rilascio numero= ',ii,' part id= ',idscint(ii)
         write(*,*)'x,y,z  in= ',xinscint(ii),yinscint(ii),zinscint(ii)
         write(*,*)'x,y,z out= ',xoutscint(ii),youtscint(ii),
     &        zoutscint(ii)
         write(*,*)'px,py,pz  in= ',pxinscint(ii),pyinscint(ii),
     &        pzinscint(ii)
         write(*,*)'px,py,pz out= ',pxoutscint(ii),pyoutscint(ii),
     &        pzoutscint(ii)
         write(*,*)'de= ',descint(ii),' al= ',alscint(ii),
     &        ' time= ',timscint(ii)
         write(*,*)
      end do
c
      return
      end
c
c     
c----------------------------------------------------
      subroutine dump_common_cross() 
c-----------------------------------------------------
c
      integer ii
      include 'mgdraw.inc'
c
      write(*,*)' '
      write(*,*)'------- dump_common_cross ---------'
      write(*,*)' '
c
      write(*,*)'numero crossing: ',ncross
      do ii = 1,ncross
         write(*,*)'crossing = ',ii,' part id = ',idcross(ii)
         write(*,*)'x,y,z = ',xcross(ii),ycross(ii),zcross(ii)
         write(*,*)'px,py,pz  in = ',pxcross(ii),pycross(ii),
     &        pzcross(ii)
         write(*,*)'reg = ',nregcross(ii),' regold = ',nregoldcross(ii),
     &        ' m = ',amacross(ii),' time = ',tcross(ii),
     &        ' cha= ',chcross(ii)
         write(*,*)
      end do      
c     
      return
      end
c
c
c----------------------------------------------------
      subroutine dump_common() 
c-----------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      include 'mgdraw.inc'
c
      if(idbflg .gt. 0) then
c         
         call dump_common_part()
c
         call dump_common_scint()
c
         call dump_common_cross()
c         
      endif
c
      return
      end
c
c      
c-------------------------------------------------------------------------
      subroutine score_startc(mreg,erawstartc,equenchedstartc,xcordin,
     &     ycordin,zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      double precision erawstartc, equenchedstartc
      include 'mgdraw.inc'
      integer ii, nstartc_now
c
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_startc-----------------'
         write(*,*)'idcurr= ',idcurr,' equenchedstartc= ',
     &        equenchedstartc
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
      endif             
c
      nstartc_now = 0
      do ii=1,nstartc
         if(idstartc(ii).eq.idcurr) nstartc_now = ii
      end do
c
      if(nstartc_now.eq.0) then
         if(nstartc.eq.maxstartc) then
            write(*,*)'Error: Score_startc:'
            write(*,*)'Maximum number of startc scoring exceeded: ev= ',
     &           ncase
            return
         endif
         nstartc = nstartc + 1
         nstartc_now = nstartc
         idstartc(nstartc) = idcurr
         xinstartc(nstartc_now)  = sngl(xcordin)
         yinstartc(nstartc_now)  = sngl(ycordin)
         zinstartc(nstartc_now)  = sngl(zcordin)
         pxinstartc(nstartc_now) = sngl(ptrack*cxtrck)
         pyinstartc(nstartc_now) = sngl(ptrack*cytrck)
         pzinstartc(nstartc_now) = sngl(ptrack*cztrck)
         timstartc(nstartc_now)  = sngl(atrack)
      endif
c 
      xoutstartc(nstartc_now) = sngl(xcordout)
      youtstartc(nstartc_now) = sngl(ycordout)
      zoutstartc(nstartc_now) = sngl(zcordout)
      pxoutstartc(nstartc_now) = sngl(ptrack*cxtrck)
      pyoutstartc(nstartc_now) = sngl(ptrack*cytrck)
      pzoutstartc(nstartc_now) = sngl(ptrack*cztrck)
      destartc(nstartc_now) = destartc(nstartc_now) + sngl(erawstartc)
      alstartc(nstartc_now)=alstartc(nstartc_now)+ sngl(equenchedstartc)
c
      if(idbflg.gt.1) then
         write(*,*)'nstartc= ',nstartc_now,
     &         ' destartc(nstartc)= ',destartc(nstartc_now)
         write(*,*)' '
      endif             
c
      return
      end
c
c
c-------------------------------------------------------------------------
      subroutine score_1DC(mreg,eraw1DC,equenched1DC,
     &     xcordin,ycordin,zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      double precision eraw1DC, equenched1DC
      include 'mgdraw.inc'
      integer ii, n1DC_now
      integer iview, ipla, icell
c
      iview = ireg2view1DC(mreg)
      ipla  = ireg2pla1DC(mreg)
      icell = ireg2cell1DC(mreg)
c
      if ((ipla.gt.6.or.ipla.lt.1).or.(icell.gt.6.or.icell.lt.1).or.
     &     (iview.gt.2.or.iview.lt.1)) then
         write(*,*) ' WARNING!!!! ipla, iview, icell, mreg '
         write(*,*) ipla, iview, icell, mreg
      endif
c     
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_1DC-----------------'
         write(*,*)'idcurr= ',idcurr,' equenched1DC= ',
     &        equenched1DC,' view= ', iview,' pla= ', ipla,
     &       ' cell= ',icell,' mreg= ',mreg
         write(*,*)'xyz= ',xcordin,ycordin,zcordin
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
      endif         
c
c new hit in the first drift chamber?   
c
      n1DC_now = 0
      do ii=1,n1DC
         if( (id1DC(ii).eq.idcurr) .and. (iview1DC(ii).eq.iview) .and.
     &        (ipla1DC(ii).eq.ipla) .and. (icell1DC(ii).eq.icell) )then
            n1DC_now = ii
         endif
      end do
c
c if ndc_now = 0 then ... new hit...
c
      if(n1DC_now.eq.0) then
         if(n1DC.eq.max1DC) then
            write(*,*)'Error: Score_1DC:'
            write(*,*)'Maximum number of 1DC scoring exceeded: ev= ',
     &           ncase
            return
         endif
         n1DC = n1DC + 1
         n1DC_now = n1DC
         id1DC(n1DC) = idcurr
         xin1DC(n1DC_now)  = sngl(xcordin)
         yin1DC(n1DC_now)  = sngl(ycordin)
         zin1DC(n1DC_now)  = sngl(zcordin)
         pxin1DC(n1DC_now) = sngl(ptrack*cxtrck)
         pyin1DC(n1DC_now) = sngl(ptrack*cytrck)
         pzin1DC(n1DC_now) = sngl(ptrack*cztrck)
         tim1DC(n1DC_now)  = sngl(atrack)
         iview1DC(n1DC_now) = iview
         ipla1DC(n1DC_now)  = ipla
         icell1DC(n1DC_now) = icell
      endif
c 
      xout1DC(n1DC_now) = sngl(xcordout)
      yout1DC(n1DC_now) = sngl(ycordout)
      zout1DC(n1DC_now) = sngl(zcordout)
      pxout1DC(n1DC_now) = sngl(ptrack*cxtrck)
      pyout1DC(n1DC_now) = sngl(ptrack*cytrck)
      pzout1DC(n1DC_now) = sngl(ptrack*cztrck)
      de1DC(n1DC_now) = de1DC(n1DC_now) + sngl(eraw1DC)
      al1DC(n1DC_now)=al1DC(n1DC_now)+ sngl(equenched1DC)
c
      if(idbflg.gt.1) then
         write(*,*)'n1DC= ',n1DC_now,
     &         ' de1DC(n1DC)= ',de1DC(n1DC_now)
         write(*,*)' '
      endif             
c    
      return
      end
c
c-------------------------------------------------------------------------
      subroutine score_vtx(mreg, erawvtx, equenchedvtx,xcordin,
     &     ycordin,zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      double precision erawvtx, equenchedvtx
      include 'mgdraw.inc'
      integer mreg, ii
      integer ipla
      integer coeff
      coeff = 0
c
      ipla = mreg-nregfirstvtx+1
      if (ipla.gt.5.or.ipla.lt.1) THEN
         write(*,*) ' WARNING!!!! iplavtx= ',ipla,
     &    '  zcordin= ',zcordin
      ENDIF
c      
      nvtx_now = 0
c
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_vtx-----------------'
         write(*,*)'test ','idcurr= ',idcurr,' pla= ',
     &        ipla, ' mreg= ',mreg,' nvtx= ',nvtx
         write(*,*)'xyz= ',xcordin,ycordin,zcordin
      endif             
c
c new hit in the vertex?
c
      if (idbflg.gt.1) then
         write(*,*)'Check of previous hits nvtx_now= ',nvtx_now,
     &        ' nvtx= ',nvtx
      endif
      do ii=1,nvtx
         if (idbflg.gt.1) then
            write(*,*)'ii= ',ii
            write(*,*)'idvtx(ii)= ',idvtx(ii),' idcurr= ',idcurr
            write(*,*)'ipla= ',ipla
            write(*,*)'irow(ii)= ',irowvtx(ii),' icol= ',icolvtx(ii)
         endif
         if( ( idvtx(ii).eq.idcurr .and.
     &         iplavtx(ii).eq.ipla ) .OR.
     &        ( sngl(xcordin).eq.xinvtx(ii) .and. 
     &          sngl(ycordin).eq.yinvtx(ii) .and.
     &          sngl(zcordin).eq.zinvtx(ii) )
     &        ) then 
            nvtx_now = ii
            if (idbflg.gt.1) then
               write(*,*)'Previous hit found: nvtx_now= ',nvtx_now,
     &              ' ii= ',ii
            endif
         endif
      end do
c
c if nvtx_now = 0 then ... new hit...
c
      if( nvtx_now.eq.0) then
         if(nvtx.eq.maxvtx) then
            write(*,*)'Error: Score_vtx:'
            write(*,*)'Maximum hit number exceeded : ev= ',ncase
            return
         endif
         nvtx = nvtx + 1
         nvtx_now = nvtx
         idvtx(nvtx) = idcurr
         iplavtx(nvtx_now) = ipla
         xinvtx(nvtx_now)  = sngl(xcordin)
         yinvtx(nvtx_now)  = sngl(ycordin)
         zinvtx(nvtx_now)  = sngl(zcordin)
         pxinvtx(nvtx_now) = sngl(ptrack*cxtrck)
         pyinvtx(nvtx_now) = sngl(ptrack*cytrck)
         pzinvtx(nvtx_now) = sngl(ptrack*cztrck)
         timvtx(nvtx_now)  = sngl(atrack)
         if (iplavtx(nvtx_now).lt.4)then
            coeff = 1
         elseif (iplavtx(nvtx_now).eq.4)then
            coeff = 3
         endif
         if (ycordin.le.(-yminvtx*coeff)) then
            irowvtx(nvtx_now) = int((ycordin-yminvtx*coeff)/dyvtx)+1
         else
            irowvtx(nvtx_now) = xpixvtx*coeff
         endif
         if (xcordin.le.(-xminvtx*coeff)) then
            icolvtx(nvtx_now) = int((xcordin-xminvtx*coeff)/dxvtx)+1
         else
            icolvtx(nvtx_now) = ypixvtx*coeff
         endif
         if (irowvtx(nvtx_now).gt.(ypixvtx*coeff)
     &        .or. irowvtx(nvtx_now).lt.1)then
            write(*,*)'WARNING! max vertex row no. exceeded: irow= ',
     &           irowvtx(nvtx_now),'yin= ',yinvtx(nvtx_now),
     &           ' mreg= ',mreg,' coeff= ',coeff
         endif
         if (icolvtx(nvtx_now).gt.(ypixvtx*coeff)
     &        .or. icolvtx(nvtx_now).lt.1)then
            write(*,*)'WARNING! max vertex col no. exceeded: icol= ',
     &           icolvtx(nvtx_now),'xin= ',xinvtx(nvtx_now),
     &           ' mreg= ',mreg,' coeff= ',coeff
         endif
      endif
c 
      xoutvtx(nvtx_now) = sngl(xcordout)
      youtvtx(nvtx_now) = sngl(ycordout)
      zoutvtx(nvtx_now) = sngl(zcordout)
      pxoutvtx(nvtx_now) = sngl(ptrack*cxtrck)
      pyoutvtx(nvtx_now) = sngl(ptrack*cytrck)
      pzoutvtx(nvtx_now) = sngl(ptrack*cztrck)
      devtx(nvtx_now) = devtx(nvtx_now) + sngl(erawvtx)
      alvtx(nvtx_now) = alvtx(nvtx_now) + sngl(equenchedvtx)
c
c
      if(idbflg.gt.1) then
         do ii=1,nvtx
            write(*,*)'ii= ',ii
            write(*,*)'idvtx(ii)= ',idvtx(ii),' idcurr= ',idcurr
            write(*,*)'ipla= ',iplavtx(ii)
            write(*,*)'irow(ii)= ',irowvtx(ii),' icol= ',icolvtx(ii)
         enddo
         write(*,*)'nvtx_now= ',nvtx_now,' nvtx= ',nvtx,
     &         ' devtx(nvtx)= ',devtx(nvtx_now)
         write(*,*)' '
      endif             
c
      return
      end
c
c      
c
c-------------------------------------------------------------------------
      subroutine score_2DC(mreg,eraw2DC,equenched1DC,xcordin,
     &     ycordin,zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      double precision eraw2DC, equenched2DC
      include 'mgdraw.inc'
      integer ii, n2DC_now
      integer iview, ipla, icell
c
      iview = ireg2view2DC(mreg)
      ipla = ireg2pla2DC(mreg)
      icell = ireg2cell2DC(mreg)
c
      if ((ipla.gt.6.or.ipla.lt.1).or.(icell.gt.6.or.icell.lt.1).or.
     &     (iview.gt.2.or.iview.lt.1)) then
         write(*,*) ' WARNING!!!! ipla, iview, icell, mreg '
         write(*,*) ipla, iview, icell, mreg
      endif
c     
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_2DC-----------------'
         write(*,*)'idcurr= ',idcurr,' equenched2DC= ',
     &        equenched2DC,' view= ', iview,' pla= ', ipla,
     &       ' cell= ',icell,' mreg= ',mreg
         write(*,*)'xyz= ',xcordin,ycordin,zcordin
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
      endif         
c
c new hit in the second drift chamber?   
c
      n2DC_now = 0
      do ii=1,n2DC
         if( (id2DC(ii).eq.idcurr) .and. (iview2DC(ii).eq.iview) .and.
     &        (ipla2DC(ii).eq.ipla) .and. (icell2DC(ii).eq.icell) )then
            n2DC_now = ii
         endif
      end do
c
c if ndc_now = 0 then ... new hit...
c
      if(n2DC_now.eq.0) then
         if(n2DC.eq.max2DC) then
            write(*,*)'Error: Score_2DC:'
            write(*,*)'Maximum number of 2DC scoring exceeded: ev= ',
     &           ncase
            return
         endif
         n2DC = n2DC + 1
         n2DC_now = n2DC
         id2DC(n2DC) = idcurr
         xin2DC(n2DC_now)  = sngl(xcordin)
         yin2DC(n2DC_now)  = sngl(ycordin)
         zin2DC(n2DC_now)  = sngl(zcordin)
         pxin2DC(n2DC_now) = sngl(ptrack*cxtrck)
         pyin2DC(n2DC_now) = sngl(ptrack*cytrck)
         pzin2DC(n2DC_now) = sngl(ptrack*cztrck)
         tim2DC(n2DC_now)  = sngl(atrack)
         iview2DC(n2DC_now) = iview
         ipla2DC(n2DC_now)   = ipla
         icell2DC(n2DC_now) = icell
      endif
c 
      xout2DC(n2DC_now) = sngl(xcordout)
      yout2DC(n2DC_now) = sngl(ycordout)
      zout2DC(n2DC_now) = sngl(zcordout)
      pxout2DC(n2DC_now) = sngl(ptrack*cxtrck)
      pyout2DC(n2DC_now) = sngl(ptrack*cytrck)
      pzout2DC(n2DC_now) = sngl(ptrack*cztrck)
      de2DC(n2DC_now) = de2DC(n2DC_now) + sngl(eraw2DC)
      al2DC(n2DC_now)=al2DC(n2DC_now)+ sngl(equenched2DC)
c
      if(idbflg.gt.1) then
         write(*,*)'n2DC= ',n2DC_now,
     &         ' de2DC(n2DC)= ',de2DC(n2DC_now)
         write(*,*)' '
      endif             
c
      return
      end
c
c     
c-------------------------------------------------------------------------
      subroutine score_scint(mreg,erawscint,equenchedscint,xcordin,
     &     ycordin,zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      double precision erawscint, equenchedscint
      include 'mgdraw.inc'
      integer ii, nscint_now
      integer rowscint, colscint
c
      rowscint = ireg2rowscint(mreg)
      colscint = ireg2colscint(mreg)
      if (colscint.gt.11 .or. colscint.lt.1) THEN
         write(*,*) ' WARNING!!!! colscint = ', colscint,
     &        ' ycordin= ',ycordin
      ENDIF
      if (rowscint.gt.11 .or. rowscint.lt.1) THEN
         write(*,*) ' WARNING!!!! rowscint = ', rowscint,
     &        ' xcordin= ',xcordin
      ENDIF
c
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_scint-----------------'
         write(*,*)'idcurr= ',idcurr,' equenchedscint= ',
     &        equenchedscint
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
         write(*,*)'mreg= ',mreg,' row= ',rowscint,' col= ',
     &        colscint 
         write(*,*)'xyz= ',xcordin,ycordin,zcordin
      endif             
c
      nscint_now = 0
      do ii=1,nscint
         if((idscint(ii).eq.idcurr.and.   
     &        irowscint(ii).eq.rowscint.and.
     &        icolscint(ii).eq.colscint ) .OR.
     &        ( sngl(xcordin).eq.xinscint(ii) .and. 
     &        sngl(ycordin).eq.yinscint(ii) .and.
     &        sngl(zcordin).eq.zinscint(ii) ))then
            nscint_now = ii
         endif
      end do
c
      if(nscint_now.eq.0) then
         if(nscint.eq.maxscint ) then
            write(*,*)'Error: Score_scint:'
            write(*,*)'Maximum number of scint scoring exceeded : ev= ',
     &           ncase
            return
         endif
         nscint = nscint + 1
         nscint_now = nscint
         idscint(nscint) = idcurr
         irowscint(nscint_now) = rowscint
         icolscint(nscint_now) = colscint
         xinscint(nscint_now)  = sngl(xcordin)
         yinscint(nscint_now)  = sngl(ycordin)
         zinscint(nscint_now)  = sngl(zcordin)
         pxinscint(nscint_now) = sngl(ptrack*cxtrck)
         pyinscint(nscint_now) = sngl(ptrack*cytrck)
         pzinscint(nscint_now) = sngl(ptrack*cztrck)
         timscint(nscint_now)  = sngl(atrack)
      endif
c 
      xoutscint(nscint_now) = sngl(xcordout)
      youtscint(nscint_now) = sngl(ycordout)
      zoutscint(nscint_now) = sngl(zcordout)
      pxoutscint(nscint_now) = sngl(ptrack*cxtrck)
      pyoutscint(nscint_now) = sngl(ptrack*cytrck)
      pzoutscint(nscint_now) = sngl(ptrack*cztrck)
      descint(nscint_now) = descint(nscint_now) + sngl(erawscint)
      alscint(nscint_now) = alscint(nscint_now) + sngl(equenchedscint)
c
      if(idbflg.gt.1) then
         write(*,*)'nscint= ',nscint_now,
     &         ' descint(nscint)= ',descint(nscint_now)
         write(*,*)'irowscint= ',irowscint(nscint_now),
     &        'icolscint= ',icolscint(nscint_now)
         write(*,*)' '
      endif             
c
      return
      end
c
c
c-------------------------------------------------------------------------
      subroutine score_cry(mreg, erawcry, equenchedcry,xcordin,ycordin,
     &     zcordin,xcordout,ycordout,zcordout)
c--------------------------------------------------------------------------
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      INCLUDE '(LTCLCM)'
      double precision erawcry, equenchedcry
      include 'mgdraw.inc'
      integer ii, mreg, ncry_now
      integer rowcry, colcry
c
      rowcry = ireg2rowcry(mreg)
      colcry = ireg2colcry(mreg)
      if (colcry.gt.11 .or. colcry.lt.1) THEN
         write(*,*) ' WARNING!!!! colcry = ', colcry
      ENDIF
      if (rowcry.gt.11 .or. rowcry.lt.1) THEN
         write(*,*) ' WARNING!!!! rowcry = ', rowcry
      ENDIF
      
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'---------------Score_cry-----------------'
         write(*,*)'idcurr= ',idcurr,' equenchedlyso= ',
     &        equenchedcry
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
      endif
c
      
      ncry_now = 0
c     
c new hit in the calo?
c
      do ii=1,ncry
         if( ((idcry(ii).eq.idcurr) .and. (irowcry(ii).eq.rowcry) .and.
     &        (icolcry(ii).eq.colcry)).or.
     &        ( sngl(xcordin).eq.xinscint(ii) .and. 
     &        sngl(ycordin).eq.yinscint(ii) .and.
     &        sngl(zcordin).eq.zinscint(ii) )) then
            ncry_now = ii
         endif
      end do
c
      if(ncry_now.eq.0) then
         if(ncry.eq.maxcry) then
            write(*,*)'Error: Score_cry:'
            write(*,*)'Maximum number of lyso scoring exceeded : ev= ',
     &           ncase
            return
         endif
         ncry = ncry + 1
         ncry_now = ncry
         idcry(ncry_now) = idcurr
         irowcry(ncry_now) = rowcry
         icolcry(ncry_now) = colcry
         xincry(ncry_now)  = sngl(xcordin)
         yincry(ncry_now)  = sngl(ycordin)
         zincry(ncry_now)  = sngl(zcordin)
         pxincry(ncry_now) = sngl(ptrack*cxtrck)
         pyincry(ncry_now) = sngl(ptrack*cytrck)
         pzincry(ncry_now) = sngl(ptrack*cztrck)
         timcry(ncry_now)  = sngl(atrack)

      endif
c 
      xoutcry(ncry_now) = sngl(xcordout)
      youtcry(ncry_now) = sngl(ycordout)
      zoutcry(ncry_now) = sngl(zcordout)
      pxoutcry(ncry_now) = sngl(ptrack*cxtrck)
      pyoutcry(ncry_now) = sngl(ptrack*cytrck)
      pzoutcry(ncry_now) = sngl(ptrack*cztrck)
      decry(ncry_now) = decry(ncry_now) + sngl(erawcry)
      alcry(ncry_now) = alcry(ncry_now) + sngl(equenchedcry)
      
      if(idbflg.gt.0) then
         write(*,*)' '
         write(*,*)'---------------Score_cry-----------------'
         write(*,*)'idcurr= ',idcurr,' equenchedlyso= ',
     &        equenchedcry
         write(*,*)'dtrack= ',(dtrack(ii),ii=1,mtrack)
      endif      
      if(idbflg.gt.1) then
         write(*,*)'ncry= ',ncry_now,
     &        ' decry(ncry)= ',decry(ncry_now)
         write(*,*)'irowcry= ',irowcry(ncry_now),
     &        'icolcry= ',icolcry(ncry_now)
         write(*,*)' '
      endif             
c
      return
      end
c
c          
c-------------------------------------------------------------------------
      SUBROUTINE score_cross(
     &        icharge,numbar,ampart,newreg,mreg,xsco,ysco,zsco)
c--------------------------------------------------------------------------
c
      INCLUDE '(DBLPRC)'
      INCLUDE '(DIMPAR)'
      INCLUDE '(IOUNIT)'
      INCLUDE '(TRACKR)'
      include 'mgdraw.inc'
      integer  icharge, numbar, newreg, mreg 
      double precision ampart, xsco, ysco, zsco
c
      if(ncross.ge.maxcross) then
         write(*,*)"SCORE_CROSS: max number of crossing exceeded"
         return
      endif
      ncross = ncross + 1
      idcross(ncross) = idcurr
      nregcross(ncross) = newreg
      nregoldcross(ncross) = mreg
      xcross(ncross)  = sngl(xsco)
      ycross(ncross)  = sngl(ysco)
      zcross(ncross)  = sngl(zsco)
      pxcross(ncross) = sngl(ptrack*cxtrck)
      pycross(ncross) = sngl(ptrack*cytrck)
      pzcross(ncross) = sngl(ptrack*cztrck)
      tcross(ncross)  = sngl(atrack)
      chcross(ncross) = icharge
      amacross(ncross) = sngl(ampart)
c     
      if(idbflg.gt.1) then
         write(*,*)' '
         write(*,*)'--------------- Score_cross -----------------'
         write(*,*)'idcurr = ',idcurr,' ncross= ',ncross
         write(*,*)'reg= ',newreg,' mreg= ',mreg,
     &        ' pxy,z= ',pxcross(ncross),
     &        pycross(ncross),pzcross(ncross),' mass= ',ampart
         write(*,*)'x,y,zcross= ',xsco,ysco,zsco,' t= ',atrack,
     &        ' cha= ',icharge
      endif             
c
      return
      end

