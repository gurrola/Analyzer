#########################################################################################################
Code developed by: Andres Florez (Los Andes), Alfredo Gurrola (Vanderbilt) and Amandeep Kalsi (Panjab)
##########################################################################################################

The recipe is to install the code at LPC/ACCRE. 
Therefore, to install the code elsewere, you'd need 
to make the necessary changes.

In order to use the code you need to follow carefully these steps:

#####################################################################
#                         CMSSW_7_4_4 Recipe                        #
#####################################################################

1. You need to work at the SLC6 area.

2. set the CMSSW release: cmsrel CMSSW_7_4_4

3. cd CMSSW_7_4_4/src

4. cmsenv

5. Install and compile the code to use the PAT-electron ID:

  5.1 git cms-merge-topic ikrav:egm_id_74X_v0  <br>
  5.2 scram b -j 10 <br>

6. Get the NTupleMaker code and compile it:
  
  6.1 git clone https://github.com/florez/NtupleMaker_740 <br>
  6.2 mv NtupleMaker_740 NtupleMaker <br>
  6.3 scram b -j 10 <br>

7. Get the Analyzer code and compile it (code that performs the analysis and produces histograms):

  7.1 git clone https://github.com/gurrola/Analyzer <br>
  7.2 scram b -j 10 <br>

8. cd into the appropriate NtupleMaker directory to start running the code:

  8.1 cd NtupleMaker/BSM3G_TNT_Maker/python <br>
  
9. Run the NtupleMaker code. In the python directory, there is file called miniAOD_*.py  <br>
   (name specifies which file is appropriate)

  9.1 cmsRun miniAOD_747_50ns.py <br>
  
10. cd into the appropriate Analyzer directory to start running the code:

  10.1 cd Analyzer/BSM3G_TNT_MainAnalyzer/ <br>

11. Copy the Ntuple root file from step 9 (or some previously produced Ntuple) and compile the code:

  11.1 cp $CMSSW_BASE/src/NtupleMaker/BSM3G_TNT_Maker/python/OutTree.root . <br>
  11.2 make clean <br>
  11.3 make BSM3GAnalyzer <br>
  
12. Run the analyzer code and inspect the output root file:

  12.1 ./BSM3GAnalyzer OutTree.root analysis.root <br>
  12.2 root -l analysis.root <br>
  
13. To run over multiple Ntuple files at LPC using CONDOR, please go to this link and follow the instructions: <br>
    https://github.com/florez/CONDOR <br>
    Please make sure to read carefully the scripts to understand how to configure the code. <br>


#####################################################################
#                         CMSSW_7_4_7 Recipe                        #
#####################################################################

1. You need to work at the SLC6 area.

2. set the CMSSW release: cmsrel CMSSW_7_4_7

3. cd CMSSW_7_4_7/src

4. cmsenv

5. Install and compile the code to use the PAT-electron ID:

  5.1 git cms-merge-topic ikrav:egm_id_747_v2 <br>
  5.2 scram b -j 10 <br>

6. Get the NTupleMaker code and compile it:

  6.1 git clone https://github.com/florez/NtupleMaker_740 <br>
  6.2 mv NtupleMaker_740 NtupleMaker <br>
  6.3 scram b -j 10 <br>

7. Get the Analyzer code and compile it (code that performs the analysis and produces histograms):

  7.1 git clone https://github.com/gurrola/Analyzer <br>
  7.2 scram b -j 10 <br>

8. cd into the appropriate NtupleMaker directory to start running the code:

  8.1 cd NtupleMaker/BSM3G_TNT_Maker/python <br>
  
9. Run the NtupleMaker code. In the python directory, there is file called miniAOD_*.py  <br>
   (name specifies which file is appropriate)

  9.1 cmsRun miniAOD_747_50ns.py <br>
  
10. cd into the appropriate Analyzer directory to start running the code:

  10.1 cd Analyzer/BSM3G_TNT_MainAnalyzer/ <br>

11. Copy the Ntuple root file from step 9 (or some previously produced Ntuple) and compile the code:

  11.1 cp $CMSSW_BASE/src/NtupleMaker/BSM3G_TNT_Maker/python/OutTree.root . <br>
  11.2 make clean <br>
  11.3 make BSM3GAnalyzer <br>
  
12. Run the analyzer code and inspect the output root file:

  12.1 ./BSM3GAnalyzer OutTree.root analysis.root <br>
  12.2 root -l analysis.root <br>
  
13. To run over multiple Ntuple files at LPC using CONDOR, please go to this link and follow the instructions: <br>
    https://github.com/florez/CONDOR <br>
    Please make sure to read carefully the scripts to understand how to configure the code. <br>


#####################################################################
#                         CMSSW_7_4_15 Recipe                        #
#####################################################################

1. You need to work at the SLC6 area.

2. set the CMSSW release: cmsrel CMSSW_7_4_15

3. cd CMSSW_7_4_15/src

4. cmsenv

5. Install and compile the code to use the PAT-electron ID:

  5.1 git cms-merge-topic ikrav:egm_id_7.4.12_v1 <br>
  5.2 scram b -j 10 <br>
  
6. Install and compile the code to use the MET producers (for covariance matrix):

  6.1 git cms-addpkg RecoMET/METProducers <br>
  6.2 scram b -j 10 <br>

7. Get the NTupleMaker code and compile it:

  7.1 git clone https://github.com/florez/NtupleMaker_740 <br>
  7.2 cd NtupleMaker_740 <br>
  7.3 git checkout miniAODv2_ForHiggsDalitz <br>
  7.4 cd ./.. <br>
  7.5 mv NtupleMaker_740 NtupleMaker <br>
  7.6 scram b -j 10 <br>

8. Get the Analyzer code and compile it (code that performs the analysis and produces histograms):

  8.1 git clone https://github.com/gurrola/Analyzer <br>
  8.2 cd Analyzer <br>
  8.3 git checkout forMiniAODv2_12142015 <br>
  8.4 cd ./.. <br>
  8.5 scram b -j 10 <br>

9. cd into the appropriate NtupleMaker directory to start running the code:

  9.1 cd NtupleMaker/BSM3G_TNT_Maker/python <br>
  
10. Run the NtupleMaker code. In the python directory, there is file called miniAODv2_7415_*.py  <br>
   (name specifies which file is appropriate)

  10.1 cmsRun miniAODv2_7415_25ns_MC.py <br>
  
11. cd into the appropriate Analyzer directory to start running the code:

  11.1 cd Analyzer/BSM3G_TNT_MainAnalyzer/ <br>

12. Copy the Ntuple root file from step 10 (or some previously produced Ntuple) and compile the code:

  12.1 cp $CMSSW_BASE/src/NtupleMaker/BSM3G_TNT_Maker/python/OutTree.root . <br>
  12.2 make clean <br>
  12.3 make BSM3GAnalyzer <br>
  
13. Run the analyzer code and inspect the output root file:

  13.1 ./BSM3GAnalyzer OutTree.root analysis.root <br>
  13.2 root -l analysis.root <br>
  
14. To run over multiple Ntuple files at LPC using CONDOR, please go to this link and follow the instructions: <br>
    https://github.com/florez/CONDOR <br>
    Please make sure to read carefully the scripts to understand how to configure the code. <br>

