#Currently histograms are limited to 1024 bins. The first value of each user input data pair is treated as the upper edge of the histogram bin and the second value is the bin content. The exception is the very first data pair the user input whose first value is the treated as the lower edge of the first bin of the histogram, and the second value is not used. This rule applies to all distribution histograms, as well as histograms for biasing.


import numpy as np
import math
import matplotlib.pyplot as plt
minEnergy=1 #meV # always use meV because fit parameters are in meV
maxEnergy=10000 #meV
numBinning=1000 #number of samples to generate
if maxEnergy-minEnergy<=100:
  E = np.linspace(minEnergy, maxEnergy, numBinning) # always use meV because fit parameters are in meV
else:
  E = np.geomspace(minEnergy, maxEnergy, numBinning) # always use meV because fit parameters are in meV, data evenly spaced in log space

if len(E)>1024:
  print("WARNING: gps istograms are limited to 1024 bins")

def fluxRiv(x):
 # A=8.057967e+06
  #kT=23.030774
  #nsd=11.050847
  #x0=55.787719
  #l=0.039221
  #B=1.050770e+07
 # a=0.057150

  A=11.9067633551420755
  kT=25
  nsd= 6.537114729381894
  x0= 46.77489256496339
  l = 0.04227477451543279
  B = 0.1891192988405625
  a = 0.04
 # incomingBeam=((A*x*np.exp(-x/kT))/(kT)**2)*(1-np.exp(-1*nsd/np.sqrt(x)))+((1+math.erf(l*(x-x0)))/2)*B*x**(a-1.5)
  incomingBeam=(((A*x*np.exp(-x/kT))/(4*np.pi*(kT)**2))+((1+math.erf(l*(x-x0)))/2)*B*x**(a-1))*(1-np.exp(-1*nsd/np.sqrt(x)))
  return incomingBeam

FluxRiv = np.vectorize(fluxRiv)(E)
integral = np.trapz(FluxRiv, E)
FluxRiv=FluxRiv/integral
normalized = np.trapz(FluxRiv, E)
if normalized == 1:
  print("correctly normalized!")
else:
  print("WARNING: FLux is not normalized, incompatible with GPS")
# set-up the figure
plt.figure(figsize=(7,7))
plt.tick_params(axis='both', which='major', labelsize="x-large")
#plt.ylim(0,150)
plt.xscale("log")
plt.yscale("log")
plt.xlabel('Energy [meV]', fontsize="x-large")
plt.ylabel('Flux [meV^-1]', fontsize="x-large")
#plt.title("Total cross section per ATOM in the molecule")
plt.grid()

plt.plot(E, FluxRiv, label="Flux")   #integral da beamcounts 1meV a 1e7 10842862.0 !!!  da 1 a 1e4 data=10452201.0 fitted=10379752.475042406
np.savetxt('FluxFit_normalized.txt', np.column_stack((E,FluxRiv)))

plt.legend()
plt.show()

EMeV=E*10**(-9) #nb Energy is saved in MeV as this is default unit for Geant4

FluxRiv=np.insert(FluxRiv,0,0)
EMeV=np.insert(EMeV,-1, EMeV[-1]-EMeV[-2])
np.savetxt('NeutronFluxVESUVIO.txt', np.column_stack((EMeV, FluxRiv)), fmt='%.8e', delimiter='\t') #header='E\tFluxRiv' to add names to columns
