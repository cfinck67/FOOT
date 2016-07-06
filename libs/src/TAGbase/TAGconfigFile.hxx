#ifndef _TAGconfigFile_HXX
#define _TAGconfigFile_HXX
/*!
  \file
  \version $Id: TAGconfigFile.hxx,v 1.1 2003/06/15 18:22:01 mueller Exp $
  \brief   Declaration of TAGconfigFile.
*/
/*------------------------------------------+---------------------------------*/

#include <fstream>

#include "TString.h"

#include "TAGnamed.hxx"

class TAGconfigFile : public TAGnamed {
  public:
                    TAGconfigFile();
    virtual         ~TAGconfigFile();

    Bool_t          Open(const TString& name);
    void            Close();

    Bool_t          IsOpen();

    Bool_t          NextLine();
    Bool_t          NextLine(TString& line);

    const TString&  CurrentLine() const;

    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    ClassDef(TAGconfigFile,0)

  private:
                    TAGconfigFile(const TAGconfigFile &);
     void           operator=(const TAGconfigFile &);

  private:
    ifstream        fStream;
    TString         fLine;
};

#include "TAGconfigFile.icc" 

#endif
