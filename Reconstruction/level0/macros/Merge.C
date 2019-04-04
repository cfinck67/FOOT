//to run root -l -b- q 'Merge.C(<filetomerge>)' (gtraini)


void Merge(const char *filename){

  FILE *stream_in = fopen("data","r"); //file template
  FILE *stream_data = fopen(filename,"r");
  FILE *stream_out = fopen("merged.dat","w");
  int ret=0;
  uint32_t word;
  int size=0;
  int nev=0;

  const uint32_t header1 = 0xeadebaba;
  const uint32_t header2 = 0x0000baba;
  const uint32_t footer1 = 0xfafefafe;
  const uint32_t footer2 = 0xbacca000;
  const uint32_t dataheader = 0x52444845;
  
  uint32_t data=0;
	
  vector<uint32_t> evt;
  int i=0;
  int size_data=0;

  const unsigned int FileHeader  = 0x1234aaaa;
  const unsigned int hFirst      = 0x1234aabb;
  const unsigned int hSecond     = 0x1234aabc;
  const unsigned int EventMarker = 0x1234cccc;
  const unsigned int EventHeader = 0xaa1234aa;
  const unsigned int ROSHeader1  = 0xdd1234dd;
  const unsigned int ROSHeader2  = 0xee1234ee;
  const unsigned int EndOfFile   = 0;
  int nw=0;

  uint32_t esize;
  while(fread(&word,4,1,stream_in)!=0){

    if(word==0xaa1234aa){
      ret = fwrite(&word,4,1,stream_out);
      if(nev==0){

	do{
	  ret = fread(&data,4,1,stream_data);
	  if(data!=dataheader)evt.push_back(data);
	}while(data!=dataheader);

	evt.push_back(dataheader);
	do{
	  ret = fread(&data,4,1,stream_data);
	  if(data!=dataheader)evt.push_back(data);
	}while(data!=dataheader);
	
      }else{
	evt.push_back(dataheader);
	do{
	  ret = fread(&data,4,1,stream_data);
	  if(data!=dataheader)evt.push_back(data);
	}while(data!=dataheader);
      }
      ret = fread(&esize,4,1,stream_in);
      esize = esize-0x247d+evt.size()+6;
      ret = fwrite(&esize,4,1,stream_out);

    }else if(word==0xeadebaba){
           
      fseek(stream_in, -8, SEEK_CUR);
      fseek(stream_out, -4, SEEK_CUR);

      ret = fread(&size,4,1,stream_in);
      
      size_data= evt.size()+6;
      ret =fwrite(&size_data,4,1,stream_out);
      ret =fwrite(&header1,4,1,stream_out);
      ret =fwrite(&header2,4,1,stream_out);
      ret =fwrite(&nev,4,1,stream_out);

      for(int i=0;i<evt.size();i++){
	word = evt.at(i);
	ret = fwrite(&word, 4,1,stream_out);
	//	if(nev==0 && (i<10 || i>evt.size()-10))printf("i::%d   wrote::%08x\n",i, evt.at(i));	
      }
      evt.clear();
      ret =fwrite(&footer1,4,1,stream_out);
      ret =fwrite(&footer2,4,1,stream_out);
      
      
      fseek(stream_in, 4*(size-2), SEEK_CUR);
      ret = fread(&word,4,1,stream_in);
      nev++;
    }else{
  
     ret = fwrite(&word,4,1,stream_out);
    }
    if(nev==200) break;
  }

  fclose(stream_in);
  fclose(stream_data);
  fclose(stream_out);
}
