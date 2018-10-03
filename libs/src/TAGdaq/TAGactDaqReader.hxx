#ifndef _TAGactDaqReader_HXX
#define _TAGactDaqReader_HXX

/*------------------------------------------+---------------------------------*/
/* TEMPLATE CLASS
   Need real implementation
*/

#include "TList.h"
#include "TString.h"

#include "TAGactionFile.hxx"

//##############################################################################

class TAGactDaqReader : public TAGactionFile {
  public:
    explicit        TAGactDaqReader(const char* name=0);
    virtual         ~TAGactDaqReader();

    void            SetupChannel(TAGdataDsc* p_data);

    virtual Int_t   Open(const TString& name, Option_t* option=0);
    virtual void    Close();

    virtual Bool_t  IsOpen() const;

    virtual Bool_t  Process();

    ClassDef(TAGactDaqReader,0)

  private:
  

};


#endif
