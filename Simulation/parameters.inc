      double precision xminVTX, yminVTX, dxVTX, dyVTX
      double precision xminITR, yminITR, dxITR, dyITR
      double precision xminMSD, yminMSD, dxMSD, dyMSD
      integer xpixVTX, ypixVTX
      integer xpixITR, ypixITR
      integer xstripMSD, ystripMSD
      integer nlayBMN, ncellBMN
      integer nlayVTX, nlayITR
      integer nlayMSD, nviewMSD
      integer nstripSCN, ncryCAL

      parameter (nlayBMN = 6)
      parameter (ncellBMN = 3)
      parameter (nlayVTX = 4)
      parameter (xminVTX = -1)
      parameter (yminVTX = -1)
      parameter (dxVTX = 0.002)
      parameter (dyVTX = 0.002)
      parameter (xpixVTX = 1000)
      parameter (ypixVTX = 1000)
      parameter (nlayITR = 2)
      parameter (xminITR = -3)
      parameter (yminITR = -3)
      parameter (dxITR = 0.002)
      parameter (dyITR = 0.002)
      parameter (xpixITR = 3000)
      parameter (ypixITR = 3000)
      parameter (nviewMSD = 2)
      parameter (nlayMSD = 3)
      parameter (xminMSD = -4.5)
      parameter (yminMSD = -4.5)
      parameter (dxMSD = 0.0125)
      parameter (dyMSD = 0.0125)
      parameter (xstripMSD = 720)
      parameter (ystripMSD = 720)
      parameter (nstripSCN = 22)
      parameter (ncryCAL = 360)
