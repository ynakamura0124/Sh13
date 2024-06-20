// ImplantedGammaSourceGeneratorAction.hh generated by R. Yokoyama on 1/29/2020

#ifndef INMPLANTED_GAMMA_SOURCE_GENERATOR_ACTION_HH
#define INMPLANTED_GAMMA_SOURCE_GENERATOR_ACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "YamlReader.hpp"
#include "G4RandomTools.hh"
#include <TDirectory.h>
#include <TChain.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TFile.h>
#include <TParameter.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TMath.h>

// This is an abstract base class for PrimaryGeneratorAction using an actual implantation
// profile from experimental ROOT files. 
// The virtual method ReadPosition() must be overrided in a derrived class.
// class T is the type of data filled in the Branch of the input TTree.
template <class T>
class ImplantedGammaSourceGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
	ImplantedGammaSourceGeneratorAction();
	virtual ~ImplantedGammaSourceGeneratorAction();

	int Configure();
	void GeneratePrimaries(G4Event *anEvent);

	virtual void SetReader() { std::cout << "virtual SetReader() called." << std::endl; };
	virtual const G4ThreeVector ReadPosition() {};
	virtual bool IsInside(const Double_t &x, const Double_t &y) const {
		return (radA_ * TMath::Power(x - x0_, 2) + radB_ * TMath::Power(y - y0_, 2) < 1);
	}

protected:
	G4double gammaE;
	G4ParticleGun* particle_gun_ = nullptr;
	TChain* input_tree_ = nullptr;

	TTreeReader* tree_reader_ = nullptr;
	TTreeReaderValue<T>* reader_value_ = nullptr;
	std::string branch_name_;
	TParameter<Bool_t>* ret_val_ = nullptr;

	Bool_t pid_gate_ = false;
	Double_t radA_;
	Double_t radB_;
	Double_t x0_;
	Double_t y0_;

};

template <class T>
ImplantedGammaSourceGeneratorAction<T>::ImplantedGammaSourceGeneratorAction() :
gammaE(0)
{
}

template <class T>
ImplantedGammaSourceGeneratorAction<T>::~ImplantedGammaSourceGeneratorAction() {
	//if (input_tree_)
	//	delete input_tree_;
	//if (particle_gun_)
	//	delete particle_gun_;
}

template <class T>
int ImplantedGammaSourceGeneratorAction<T>::Configure() {
	/* creates a particle gun for gamma */
	if (particle_gun_)
		delete particle_gun_;
	particle_gun_ = new G4ParticleGun(1);
	G4ParticleTable* particle_table = G4ParticleTable::GetParticleTable();
	particle_gun_->SetParticleDefinition(particle_table->FindParticle("gamma"));
	particle_gun_->SetParticlePosition(G4ThreeVector(0.0,0.0,0.0));

	/* reads yaml file and create a TChain instance */
	YamlReader yaml_reader("ImplantedGammaSourceGeneratorAction");

	/* sets gamma-ray energy */
	gammaE = yaml_reader.GetDouble("GammaEnergy");
	particle_gun_->SetParticleEnergy(gammaE * CLHEP::keV);

	const auto tree_name = yaml_reader.GetString("TreeName");
	if (input_tree_)
		delete input_tree_;
	{
		std::string cmd = "TChain* chain = new TChain(\"" + tree_name + "\");";
		gROOT->ProcessLine(cmd.c_str());
	}
	/* adds all rootfiles in the list to the TChain object */
	auto merger_list = yaml_reader.GetNode("MergerList");
	for (const auto& root_file : merger_list) {
		const auto file_name = root_file["FileName"].as<std::string>();
		std::string cmd = "chain->Add(\"" + file_name + "\");";
		gROOT->ProcessLine(cmd.c_str());
		std::cout << "[ImplantedGammaSourceGeneratorAction]: Added file to chain: " << file_name << std::endl;
	}
	gROOT->ProcessLine("gROOT->Append(chain);");
	input_tree_ = (TChain*)gDirectory->GetList()->At(0);
	std::cout << "[ImplantedGammaSourceGeneratorAction]: Number of files added to the chain: " << input_tree_->GetEntries() << std::endl;

	/* sets TTree Reader */
	if (tree_reader_)
		delete tree_reader_;
	gSystem->Load("libdata2TreeLib.so");
	gROOT->ProcessLine("auto reader = new TTreeReader(chain);");
	gROOT->ProcessLine("gROOT->Append(reader);");
	tree_reader_ = (TTreeReader*)gDirectory->GetList()->At(1);
	gROOT->ProcessLine("auto param = new TParameter<Bool_t>(\"ReturnVal\",false);");
	gROOT->ProcessLine("gROOT->Append(param);");
	ret_val_ = (TParameter<Bool_t>*)gDirectory->FindObject("ReturnVal");

	if (reader_value_)
		delete reader_value_;
	branch_name_ = yaml_reader.GetString("BranchName");
	reader_value_ = new TTreeReaderValue<T>(*tree_reader_,branch_name_.c_str());
	//this->SetReader();

	if (pid_gate_ = yaml_reader.GetBoolean("PIDGate",false,false)) {
		radA_ = yaml_reader.GetDouble("RadA");
		radB_ = yaml_reader.GetDouble("RadB");
		x0_ = yaml_reader.GetDouble("X0");
		y0_ = yaml_reader.GetDouble("Y0");
	}


	return 0;
}

template <class T>
void ImplantedGammaSourceGeneratorAction<T>::GeneratePrimaries( G4Event* anEvent ) {
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
	/* set source position */
	{
		particle_gun_->SetParticlePosition(this->ReadPosition());
	}
	/* emit gamma */
	particle_gun_->GeneratePrimaryVertex(anEvent);

	return;
}
#endif