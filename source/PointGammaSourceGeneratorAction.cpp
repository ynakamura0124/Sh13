#include "PointGammaSourceGeneratorAction.hh"

using namespace CLHEP;

int PointGammaSourceGeneratorAction::Configure() {
	/* create a particle gun for gamma */
	particle_gun_ = new G4ParticleGun(1);
	G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
	particle_gun_->SetParticleDefinition(particle_table->FindParticle("gamma"));
	particle_gun_->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));

	/* read yaml file and create a gamma source instance */
	YamlReader yaml_reader("PointGammaSourceGeneratorAction");
	if (gamma_source_) {
		delete gamma_source_;
		gamma_source_ = nullptr;
	}
	gamma_source_ = new GSource4G4(yaml_reader.GetString("SourceFile"));
	gamma_source_->SetNevent(yaml_reader.GetULong64("NEvent"));

	if (!gamma_source_)
		return 1;
	else
		return 0;
}

void PointGammaSourceGeneratorAction::GeneratePrimaries( G4Event* anEvent ) {
	/* set gamma-ray energy*/
	G4int n_gamma = 0;
	while (!(n_gamma = gamma_source_->EmitGamma())) {}
	for (int i = 0; i < n_gamma; ++i) {
		/* set gamma-ray direction */
		{
			/* A=-1, B=1 for 4*pi, B=0 for 2*pi emission */
			const G4double A = -1.0;
			const G4double B = 1.0;
			G4double cosTheta = G4UniformRand() * (B - A) + A;// A < cosTheta < B
			G4double sinTheta = sqrt(1.0 - cosTheta * cosTheta);// 0. < sinTheta < 1.
			G4double phi = G4UniformRand() * 8.0 * atan(1.0);// 0. < phi < 2*pi

			G4double randomXaim = cos(phi) * sinTheta;
			G4double randomYaim = sin(phi) * sinTheta;
			G4double randomZaim = cosTheta;
			G4ThreeVector aim(randomXaim, randomYaim, randomZaim);

			particle_gun_->SetParticleMomentumDirection(aim);
		}
		particle_gun_->SetParticleEnergy(gamma_source_->GetEGamma(i)*keV);
		particle_gun_->GeneratePrimaryVertex(anEvent);
	}
	return;
}