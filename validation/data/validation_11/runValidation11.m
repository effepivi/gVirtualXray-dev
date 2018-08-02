%
%
% Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
% http://www.fpvidal.net/
% All rights reserved.
%
% Redistribution and use in source and binary forms, with or without modification,
% are permitted provided that the following conditions are met:
%
% 1. Redistributions of source code must retain the above copyright notice,
% this list of conditions and the following disclaimer.
%
% 2. Redistributions in binary form must reproduce the above copyright notice,
% this list of conditions and the following disclaimer in the documentation and/or
% other materials provided with the distribution.
%
% 3. Neither the name of the Bangor University nor the names of its contributors
% may be used to endorse or promote products derived from this software without
% specific prior written permission.
%
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
% ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
% THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
% ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
% FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
% DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
% SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
% CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
% OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
% THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the image package (needed for FBP)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pkg load image




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the parallel package (needed for parallel computing)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pkg load parallel




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Add the path with octave routines
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath("../octave")




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

volume_size = [512 512 1];
voxel_size = [1.0 1.0 1.0];

number_of_angles          = 180;
first_angle               =   0.0;
angle_offset              =   1.0;
angle_span                = first_angle:angle_offset:((number_of_angles - 1) * angle_offset);
number_of_processes       = 6;




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the volume at 80 keV (i.e. 0.08 MeV)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ct_volume_in_mu_80keV = loadVolume("ct_slice.raw", "int16", volume_size(1), volume_size(2), volume_size(3));




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build corresponding sinogram
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sinogram_80keV = radon(ct_volume_in_mu_80keV, angle_span);
writeVolume("reference_sinogram.raw", "float32", sinogram_80keV);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build corresponding projections
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

projections_80keV = buildProjectionsFromSinogram(sinogram_80keV);
writeVolume("reference_projections.raw", "float32", projections_80keV);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reconstructed_ct_volume = reconstructVolume(number_of_processes, sinogram_80keV, angle_span);
reconstructed_ct_volume = cell2mat(reconstructed_ct_volume);
writeVolume("reconstructed_rerefence.raw", "float32", reconstructed_ct_volume);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the sinogram
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

test_sinogram = loadVolume("test_sinogram.raw", "float32", size(sinogram_80keV, 1), size(sinogram_80keV, 2), size(sinogram_80keV, 3));




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build corresponding projections
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

test_projections = buildProjectionsFromSinogram(test_sinogram);
writeVolume("test_projection_set.raw", "float32", test_projections);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

test_reconstructed_ct_volume = reconstructVolume(number_of_processes, test_sinogram, angle_span);
test_reconstructed_ct_volume = cell2mat(test_reconstructed_ct_volume);
writeVolume("test_reconstructed_phantom.raw", "float32", test_reconstructed_ct_volume);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sinogram error
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
sinogram_error = abs(sinogram_80keV - test_sinogram);
writeVolume("sinogram_error.raw", "float32", sinogram_error);
mean(mean(mean(sinogram_error)))
min(min(min(sinogram_error)))
max(max(max(sinogram_error)))




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Projection error
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
projection_error = abs(projections_80keV - test_projections);
writeVolume("projection_error.raw", "float32", projection_error);
mean(mean(mean(projection_error)))
min(min(min(projection_error)))
max(max(max(projection_error)))




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction error
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reconstruction_error = abs(ct_volume_in_mu_80keV - test_reconstructed_ct_volume);
writeVolume("reconstruction_error.raw", "float32", reconstruction_error);
mean(mean(mean(reconstruction_error)))
min(min(min(reconstruction_error)))
max(max(max(reconstruction_error)))




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract profiles
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reference_sinogram_profile = sinogram_80keV(      : , int32(size     (sinogram_80keV, 2) / 2), int32(size(     sinogram_80keV, 3) / 2) );
test_sinogram_profile      = test_sinogram( : , int32(size(test_sinogram, 2) / 2), int32(size(test_sinogram, 3) / 2) );

save -ascii reference_sinogram_profile.txt reference_sinogram_profile;
save -ascii      test_sinogram_profile.txt      test_sinogram_profile;




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Save images
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Input data
figure;
imagesc(ct_volume_in_mu_80keV(: , :, int32(size(ct_volume_in_mu_80keV, 3) / 2))');
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("ct_volume_in_mu_80keV.png","-dpng"); 

% Sinograms
climits_80keV      = [min(min(min(     sinogram_80keV))), max(max(max(     sinogram_80keV)))];
climits_simulation = [min(min(min(test_sinogram))), max(max(max(test_sinogram)))];
climits = [min(climits_80keV(1), climits_simulation(1)), max(climits_80keV(2), climits_simulation(2))];

figure;
imagesc(sinogram_80keV(: , :, int32(size(sinogram_80keV, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("sinogram_80keV.png","-dpng"); 

figure;
imagesc(test_sinogram(: , :, int32(size(test_sinogram, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("test_sinogram.png","-dpng"); 

figure;
imagesc(sinogram_error(: , :, int32(size(sinogram_error, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("sinogram_error.png","-dpng"); 

% Projections
figure;
imagesc(projections_80keV(: , :, int32(size(projections_80keV, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("projections_80keV.png","-dpng"); 

figure;
imagesc(test_projections(: , :, int32(size(test_projections, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("test_projections.png","-dpng"); 

figure;
imagesc(projection_error(: , :, int32(size(projection_error, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("projection_error.png","-dpng"); 

% Reconstructions
climits_80keV      = [min(min(min(           reconstructed_ct_volume))), max(max(max(           reconstructed_ct_volume)))];
climits_simulation = [min(min(min(test_reconstructed_ct_volume))), max(max(max(test_reconstructed_ct_volume)))];
climits = [min(climits_80keV(1), climits_simulation(1)), max(climits_80keV(2), climits_simulation(2))];

figure;
imagesc(reconstructed_ct_volume(: , :, int32(size(reconstructed_ct_volume, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("reconstructed_ct_volume.png","-dpng"); 

figure;
imagesc(test_reconstructed_ct_volume(: , :, int32(size(test_reconstructed_ct_volume, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("test_reconstructed_ct_volume.png","-dpng"); 

figure;
imagesc(reconstruction_error(: , :, int32(size(reconstruction_error, 3) / 2))', climits);
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("reconstruction_error.png","-dpng"); 

% Normalised cross-correlation
slice1 = sinogram_80keV(     : , :, int32(size(     sinogram_80keV, 3) / 2));
slice2 = test_sinogram(: , :, int32(size(test_sinogram, 3) / 2));

mean1 = mean(mean(slice1));
mean2 = mean(mean(slice2));

standard_deviation1 = std2(slice1);
standard_deviation2 = std2(slice2);

normalised_cross_correlation = ((slice1 - mean1) .* (slice2 - mean2) / (standard_deviation1 * standard_deviation2));
sum(sum(normalised_cross_correlation)) / (size(slice1, 1) * size(slice1, 2))
figure;
imagesc(reconstruction_error(: , :, int32(size(reconstruction_error, 3) / 2))' / (size(slice1, 1) * size(slice1, 2)));
colormap("gray");
colorbar;
daspect( [ 1 1 1 ] );
print("reconstruction_error.png","-dpng"); 

