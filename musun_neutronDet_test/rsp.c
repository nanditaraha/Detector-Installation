void rsp(int runnr, int addr, int fadc)
{
  gInterpreter->AddIncludePath("/home/l_data/daq/midas/include");
  gROOT->LoadMacro("/home/l_data/daq/midas/linux64/lib/libmidas_shared.so");
  gROOT->LoadMacro("hdb.cxx+");
  gROOT->LoadMacro("/home/l_data/daq/online-analysis/compress/libmucap_compress.so");
   //gROOT->LoadMacro("show_pulses_serdar.cxx+");
    gROOT->LoadMacro("show_pulses_musun.cxx+");
  char name[256];
   sprintf(name, "/data/midas-musun/run%05d.mid", runnr);
  //sprintf(name, "/data/midas-mu2e/run%05d.mid", runnr);
  process_file(name, addr, fadc);
}
