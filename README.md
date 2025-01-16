#Neutron irradiation of phantom materials simulation
Thermal neutrons are used to simulate irradiation experiment. Thermal neutron cross section of hydrogen-based materials are estimated using AFGA, and calculated with the help of NCrystal, as described below.

##Geometry
From left to right (negative to positive Z) the geoometry is composed of a neutron source, a sample and a neutron detector. The world is made of galactic material, in order to only observe the contribution of the sample.

The **sample** is composed of a polymeric solid, it is possible to choose between different polymers

The **Imaging detector** is composed of two box solids, placed one in front of the other, the simulation detects neutrons that pass from one to the other, saving their energy and XY position. The detector is ideal, so it is made of the same material as the world (defined as worldMaterial).

the **neutron beam** has a squared section of 2x2 mm and impinges on the sample travelling from negative to positive Z values.

##Materials
In this version each material is defined in three different ways (e.g.: G4_WATER, G4_WATER_NC, G4_WATER_TS) Geant materials and NCrystal materials can be used by changing the definition of the variable water in the line "auto water = G4Material::GetMaterial("G4_WATER_NC");" to compare free gas model to NCrystal models. For materials that use the geant neutron thermal scattering libraries you need to use the physics list "PhysicsList" and decomment the lines on thermal scattering inside "PhysicsList.cc".

**nb.:** I don't really know if G4NeutronHPThermalScattering works as it should, transmission looks wrong but I don't know what model it's using, needs to be checked

##Physics list: Ncrystal
It is possible to use a physics list manually defined, (in the case of "physicsList.cc", it was taken from a geant4 example), or to use a pre-defined G4 physicslist that correctly treats neutrons in the thermal range, using free gas cross sections (for example QGSP_BIC_HP). Either way,when using NCrystal it is important that the physics list used has 1 active process derived from G4HadronElasticProcess for neutrons, because NCrystal takes over the pHadElastic process, redefining it according to its cross sections.

**nb.:** standard parameters for NCrystal materials are defined inside MatHelper.cc and they are: density=1.0gcm3, kStateSolid, temperature=293.15 kelvin, pressure= 1.0 atmosphere. It is important to note that they may not correspond to default geant parameters, (for example default temperature in geant is 273.15K) so it is always good practice to specify these parameters.

**nb.:** NCrystal is not thread-safe, so in the main all lines to run in multi-thread mode were commented.

**nb.:** Ncrystal takes over pHadElastic up to 5eV, as defined in the class G4NCProcWrapper.cc (even though inside the class G4NVInstall.hh is reported as 2eV). The 5eV threshold may be revisited in future versions as materials like tungsten have a resonance below 5eV.

**nb.:** NCrystal installation needs to be done after initialization of runManager, so the latter can not be done inside the macro file

##Primary generator
In this version neutrons are generated with a constant distribution, using G4UniformRand(), the beam has a squared section of 2x2 mm.

**nb.:** G4UniformRand does not vary its seed automatically, but seed can be implemented inside exampleB4c.cc


