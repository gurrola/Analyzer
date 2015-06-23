#######################################################################################################
Code developed by: Alfredo Gurrola (Vanderbilt), Andres Florez (Los Andes), Amandeep Kalsi (Panjab)
#######################################################################################################

In order to use the checkout and use the code, follow these steps:

1. You need to work at the SLC6 area.

   1.1 If you are working from the LPC cluster, you need to do the following:<br>
    source /cvmfs/cms.cern.ch/cmsset_default.csh <br>
    setenv SCRAM_ARCH slc6_amd64_gcc491
   
2. set the CMSSW release: cmsrel CMSSW_7_4_0

3. cd CMSSW_7_4_0/src

4. cmsenv

5. Install and compile the code to use the PAT-electron ID:

  5.1 git cms-merge-topic ikrav:egm_id_74X_v0 <br>
  5.2 scram b -j 10

6. Get the NTupleMaker code and compile it:
  
  6.1 git clone https://github.com/florez/NtupleMaker_740 <br>
  6.2 mv NtupleMaker_740 NtupleMaker <br>
  6.3 scram b -j 10
  
7. Get the Analyzer code and compile it (code that performs the analysis and produces histograms):

  7.1 git clone https://github.com/gurrola/Analyzer <br>
  7.2 scram b -j 10

8. Get the Plotter code and compile it (scripts to create plots and calculate efficiencies)

  8.1 git clone https://github.com/gurrola/Plotter <br>
  8.2 scram b -j 10

9. cd into the appropriate NtupleMaker directory to start running the code:

  9.1 cd NtupleMaker/BSM3G_TNT_Maker/python
  
10. Copy a test input miniAOD root file from gurrola's area (or your own):

  // FROM VANDERBILT:
  10.1 cp /home/gurrola/mini_AODSIM.root . <br>
  // FROM LPC:
  10.1 cp /uscms/home/florez/mini_AODSIM.root . <br> 
11. Run the NtupleMaker code:

  11.1 cmsRun miniAOD.py <br>
  
12. cd into the appropriate Analyzer directory to start running the code:

  12.1 cd Analyzer/BSM3G_TNT_MainAnalyzer/

13. Copy a the Ntuple root file from step 11 and compile the code:

  13.1 cp $CMSSW_BASE/src/NtupleMaker/BSM3G_TNT_Maker/python/OutTree.root . <br>
  13.2 make clean <br>
  13.3 make BSM3GAnalyzer
  
14. Run the analyzer code and inspect the output root file:

  14.1 ./BSM3GAnalyzer <br>
  14.2 root -l analysis.root
  
15. Use the Plotter code and output from running the analyzer to produce plots and a cut flow eff table:

  15.1 cd ../../Plotter/TNTCode/ <br>
  15.2 cp ../../Analyzer/BSM3G_TNT_MainAnalyzer/analysis.root Data <br>
  15.3 cp ../../Analyzer/BSM3G_TNT_MainAnalyzer/analysis.root DYMuMu <br>
  15.4 root -l -b main.C

16. Inspect the output root file:

  16.1 root -l Plots_Rebin1.root
