####################################################

Code developed by: Alfredo Gurrola (Vanderbilt)

###################################################

In order to use the checkout and use the code, follow these steps:

1. You need to work at the SLC6 area.

2. set the CMSSW release: cmsrel CMSSW_7_2_0

3. cd CMSSW_7_2_0/src

4. cmsenv

5. Install and compile the code to use the PAT-electron ID:

  5.1 git cms-merge-topic ikrav:egm_id_phys14 <br>
  5.2 scram b -j 10

6. Get the NTupleMaker code and compile it:
  
  6.1 git clone https://github.com/florez/NtupleMaker <br>
  6.2 scram b -j 10
  
7. Get the Analyzer code and compile it (code that performs the analysis and produces histograms):

  7.1 git clone https://github.com/gurrola/Analyzer <br>
  7.2 scram b -j 10

8. Get the Plotter code and compile it (scripts to create plots and calculate efficiencies)

  8.1 git clone https://github.com/gurrola/Plotter <br>
  8.2 scram b -j 10

9. cd into the appropriate Analyzer directory to start running the code:

  9.1 cd Analyzer/BSM3G_TNT_MainAnalyzer/

10. Copy a test input root file from gurrola's area and compile the code:

  10.1 cp /scratch/gurrola/CMSSW_7_2_0/src/OutTree_9_1_dab.root . <br>
  10.2 make clean <br>
  10.3 make BSM3GAnalyzer
  
11. Run the analyzer code and inspect the output root file:

  11.1 ./BSM3GAnalyzer <br>
  11.2 root -l analysis.root
  
12. Use the Plotter code and output from running the analyzer to produce plots and a cut flow eff table:

  12.1 cd ../../Plotter/TNTCode/ <br>
  12.2 cp ../../Analyzer/BSM3G_TNT_MainAnalyzer/analysis.root Data <br>
  12.3 cp ../../Analyzer/BSM3G_TNT_MainAnalyzer/analysis.root DYMuMu <br>
  12.4 root -l -b main.C

13. Inspect the output root file:

  13.1 root -l Plots_Rebin1.root
