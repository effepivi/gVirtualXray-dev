#include <string>
#include <exception>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <set>
#include <cmath>
#include <stdio.h>

#include "TApplication.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TPad.h"

using namespace std;

int main(int argc, char* argv[])
{
	FILE* p_direct_photon_output_file(0);
	float* p_direct_photon_image(0);

	FILE* p_scattered_photon_output_file(0);
	float* p_scattered_photon_image(0);

	TFile* p_input_file(0);
	TTree* singlesTree(0);
	TTree* gateTree(0);
	
	try
	{
		// Epsilon
		Double_t EPSILON(1E-6);

		// Size and number of pixel (in mm)
		Double_t const PIXELSIZE = 0.3;
		Int_t const ROW = 301;
		Int_t const COLUMN = 301;

		// Energy bounds in MeV
		Double_t const EMIN = 0.08-EPSILON;
		Double_t const EMAX = 0.08+EPSILON;
	
		// Create the buffer
		p_direct_photon_image = new float[ROW * COLUMN];
		if (!p_direct_photon_image)
		{
			throw "Cannot create a new image.";
		}
		p_scattered_photon_image = new float[ROW * COLUMN];
		if (!p_scattered_photon_image)
		{
			throw "Cannot create a new image.";
		}

		// Open the output files
		p_direct_photon_output_file = fopen("gate_simulation_direct_photons.raw", "wb");
		if (!p_direct_photon_output_file)
		{
			throw "Cannot open output file.";
		}
		p_scattered_photon_output_file = fopen("gate_simulation_scattered_photons.raw", "wb");
		if (!p_scattered_photon_output_file)
		{
			throw "Cannot open output file.";
		}		

		// Initialise the data
		for (int i(0); i < ROW * COLUMN; ++i)
		{
			p_direct_photon_image[i] = 0.0;			
			p_scattered_photon_image[i] = 0.0;
		}
		
		// Define the bounds of the image
		Double_t const ROW_BOUND =  PIXELSIZE * ROW / 2;
		Double_t const COLUMN_BOUND = PIXELSIZE * COLUMN / 2;

		unsigned int scattering_photon(0);
		unsigned int direct_photon(0);
		unsigned int total_photon_number(0);
				
		Double_t scattering_photon_total_energy(0);
		Double_t direct_photon_total_energy(0);
		Double_t total_energy(0);

		Double_t first_pixel_x_position(-COLUMN_BOUND);
		Double_t first_pixel_y_position(-ROW_BOUND);
		Double_t last_pixel_x_position(  COLUMN_BOUND);
		Double_t last_pixel_y_position(  ROW_BOUND);

		for (unsigned int run_id(0); run_id <= 59; ++run_id)
		{
			// Store the root file name in 'fileName' variable
			stringstream FILENAME;
			FILENAME << "run_" << run_id << "/benchmarkVRTCT_000.root";
	
			// Open (check) and read the root file
			p_input_file = new TFile( FILENAME.str().data() );
			if( p_input_file->IsOpen() )
			{
				// Take the single tree, where is the position, the energy and the runID
				TTree* singlesTree = (TTree*)p_input_file->Get( "Singles" );
				if (!singlesTree)
				{
					throw "Cannot get Singles";
				}
		
				// Global Position in X, Y and Z
				Float_t globalPosX, globalPosY, globalPosZ, energy;
				Int_t runID, pixelID, rayleighPhantom, comptonPhantom;
				singlesTree->SetBranchAddress( "globalPosX", &globalPosX );
				singlesTree->SetBranchAddress( "globalPosY", &globalPosY );
				singlesTree->SetBranchAddress( "globalPosZ", &globalPosZ );
				singlesTree->SetBranchAddress( "energy", &energy );
				singlesTree->SetBranchAddress( "runID", &runID );
				singlesTree->SetBranchAddress( "pixelID", &pixelID );
				singlesTree->SetBranchAddress( "RayleighPhantom", &rayleighPhantom );
				singlesTree->SetBranchAddress( "comptonPhantom", &comptonPhantom );

				// Number of entries in the single tree
				Int_t entriesSingleTree = (Int_t)singlesTree->GetEntries();

				// Number of generated photons during the benchmarkCT simulation
				gateTree = (TTree*)p_input_file->Get( "Gate" );

				// Sort the photons
				for( Int_t i = 0; i != entriesSingleTree; ++i )
				{
					singlesTree->GetEntry( i );

					// Convert the photon position into pixel position
					int pixel_x_position(((globalPosX - first_pixel_x_position) / (last_pixel_x_position - first_pixel_x_position)) * COLUMN);
					int pixel_y_position(((globalPosY - first_pixel_y_position) / (last_pixel_y_position - first_pixel_y_position)) * ROW);

					// The photon is in the detector
					if (0 <= pixel_x_position && pixel_x_position < COLUMN && 0 <= pixel_y_position && pixel_y_position < ROW)
					{
					
						// Select only directly transmitted photons
						if (EMIN <= energy && energy <= EMAX)
						{
							p_direct_photon_image[pixel_y_position * COLUMN + pixel_x_position] += energy;
				
							//cout << energy << "\t" << rayleighPhantom << "\t" << comptonPhantom << endl;
							++direct_photon;
							direct_photon_total_energy += energy;
						}
						else
						{
							p_scattered_photon_image[pixel_y_position * COLUMN + pixel_x_position] += energy;
				
							++scattering_photon;
							scattering_photon_total_energy += energy;
						}
					
						++total_photon_number;
						total_energy += energy;
					}
					else
					{
						cout << "Photon outside the detector" << endl;
					}
				}
			}

			if (p_input_file)
			{
				delete p_input_file;
				p_input_file = 0;
			}
		}
		
		cout << "Directly transmitted photons: " << direct_photon << "\t(" << 100.0 * float(direct_photon) / float(total_photon_number) << "\%)" << endl;
		cout << "Scattered photons: " << scattering_photon << "\t(" << 100.0 * float(scattering_photon) / float(total_photon_number) << "\%)" << endl;
		cout << "Total photons: " << total_photon_number << endl;

		cout << "Directly transmitted energy: " << direct_photon_total_energy << "\t(" << 100.0 * float(direct_photon_total_energy) / float(total_energy) << "\%)" << endl;
		cout << "Scattered energy: " << scattering_photon_total_energy << "\t(" << 100.0 * float(scattering_photon_total_energy) / float(total_energy) << "\%)" << endl;
		cout << "Total energy: " << total_energy << endl;
		
		// Save the data into a file
		cout << "Save data" << endl;
		for (int i(0); i < ROW; ++i)
		{
			if (fwrite(&p_direct_photon_image[i * COLUMN], sizeof(float), COLUMN, p_direct_photon_output_file) != COLUMN)
			{
				throw "Cannot save all the data.";
			}
			if (fwrite(&p_scattered_photon_image[i * COLUMN], sizeof(float), COLUMN, p_scattered_photon_output_file) != COLUMN)
			{
				throw "Cannot save all the data.";
			}
		}
		cout << "Data saved" << endl;
	}
	catch (const std::exception& e)
	{
		cerr << "ERROR: " << e.what() << endl;
	}
	catch (const string& e)
	{
		cerr << "ERROR: " << e << endl;
	}	
	catch (const char* e)
	{
		cerr << "ERROR: " << e << endl;
	}
	catch (...)
	{
		cerr << "ERROR: " << "unknown" << endl;
	}	

	if (p_direct_photon_image)
	{
		cout << "Destroy output data" << endl;
		delete[] p_direct_photon_image;
	}

	if (p_scattered_photon_image)
	{
		cout << "Destroy output data" << endl;
		delete[] p_scattered_photon_image;
	}
	
	if (p_direct_photon_output_file)
	{
		cout << "Close output file" << endl;
		fclose(p_direct_photon_output_file);
	}

	if (p_scattered_photon_output_file)
	{
		cout << "Close output file" << endl;
		fclose(p_scattered_photon_output_file);
	}

	if (p_input_file)
	{
		cout << "Close input file" << endl;
		delete p_input_file;
	}
			
	return 0;
}
