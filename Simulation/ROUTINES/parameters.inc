c     BEAM MONITOR PARAMETERS 

      integer ncellBMN
      parameter (ncellBMN = 3)
      integer nlayBMN
      parameter (nlayBMN = 6)

c     VERTEX PARAMETERS 

      integer nlayVTX
      parameter (nlayVTX = 4)
      integer xpixVTX
      parameter (xpixVTX = 928)
      integer ypixVTX
      parameter (ypixVTX = 960)
      double precision dxVTX
      parameter (dxVTX = 0.00207D+00)
      double precision dyVTX
      parameter (dyVTX = 0.00207D+00)
      double precision xminVTX
      parameter (xminVTX = -0.96048D+00)
      double precision yminVTX
      parameter (yminVTX = -0.99360D+00)

c     INTERMEDIATE TRACKER PARAMETERS 

      integer nlayITR
      parameter (nlayITR = 2)
      integer nmimoITR
      parameter (nmimoITR = 4)
      integer nplumeITR
      parameter (nplumeITR = 4)
      integer xpixITR
      parameter (xpixITR = 960)
      integer ypixITR
      parameter (ypixITR = 928)
      double precision deadITR
      parameter (deadITR = 0.03680D+00)
      double precision dxITR
      parameter (dxITR = 0.00207D+00)
      double precision dyITR
      parameter (dyITR = 0.00207D+00)
      double precision heightITR
      parameter (heightITR = 1.92096D+00)
      double precision widthITR
      parameter (widthITR = 1.98720D+00)
      double precision xminITR0(4)
      data xminITR0/-3.54700D+00,-3.54700D+00,-3.51220D+00,-3.51220D+00/
      double precision xminITR1(4)
      data xminITR1/-3.51220D+00,-3.51220D+00,-3.54700D+00,-3.54700D+00/
      double precision yminITR0(4)
      data yminITR0/-3.39192D+00,-1.77096D+00,-0.15000D+00,1.47096D+00/
      double precision yminITR1(4)
      data yminITR1/-3.39192D+00,-1.77096D+00,-0.15000D+00,1.47096D+00/

c     MICRO STRIP DETECTOR PARAMETERS 

      integer nlayMSD
      parameter (nlayMSD = 3)
      integer xstripMSD
      parameter (xstripMSD = 720)
      integer ystripMSD
      parameter (ystripMSD = 720)
      double precision dxMSD
      parameter (dxMSD = 0.01250D+00)
      double precision dyMSD
      parameter (dyMSD = 0.01250D+00)
      double precision xminMSD
      parameter (xminMSD = -4.50000D+00)
      double precision yminMSD
      parameter (yminMSD = -4.50000D+00)

c     SCINTILLATOR PARAMETERS 

      integer nstripSCN
      parameter(nstripSCN = 22)

c     CALORIMETER PARAMETERS 

      integer ncryCAL
      parameter(ncryCAL = 360)

