%!/usr/bin/octave

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




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function projections=buildProjections(aSinogram)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    width = size(aSinogram, 1);
    slice_number = size(aSinogram, 3);
    angle_number = size(aSinogram, 2);

    % Allocate the memory
    projections = zeros([width slice_number angle_number]);

    % Process all the angle
    for angle_id = 1:angle_number

        % Process all the slices
        for slice_id = 1:slice_number
            % Copy the data
            projections(:,slice_id,angle_id) = aSinogram(:,angle_id,slice_id);
        endfor
    endfor

    % Return the projections
    return;

endfunction




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function ct_slice=reconstructSlice(aSinogram,
                                   aSliceID,
                                   anAngleSpan)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    % Reconstruct the slice
    ct_slice = iradon(aSinogram(:,:, aSliceID), anAngleSpan, "Spline", "Hamming");

    % Copy the data
    ct_slice = ct_slice(2:size(ct_slice, 1) - 1,2:size(ct_slice, 2) - 1);

    return;

endfunction




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

volume_size = [256 256 128];
voxel_size = [0.5 0.5 1.0];

number_of_angles          = 360;
first_angle               =   0.0;
angle_offset              =   0.5;
angle_span                = first_angle:angle_offset:((number_of_angles - 1) * angle_offset);
number_of_processes       = 6;




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the volume in HU
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ct_volume_in_HU = loadVolume("validation_08_phantom.raw", "int16", volume_size(1), volume_size(2), volume_size(3));




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Convert HU values in mu value at 80 keV (i.e. 0.08 MeV)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

mu_water = 1.844E-01;
ct_volume_in_mu_80keV = (1.0 + ct_volume_in_HU / 1000.0) * mu_water;
writeVolume("validation_08_phantom_in_mu_80keV.raw", "float32", ct_volume_in_mu_80keV);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Build corresponding sinogram
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sinogram_80keV = buildSinogram(6, ct_volume_in_mu_80keV, angle_span);
writeVolume("validation_08_phantom_sinogram.raw", "float32", sinogram_80keV);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reconstructed_ct_volume = reconstructVolume(number_of_processes, sinogram_80keV, angle_span);
reconstructed_ct_volume = cell2mat(reconstructed_ct_volume);
writeVolume("validation_08_reconstructed_phantom.raw", "float32", reconstructed_ct_volume);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the sinogram
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

simulation_sinogram = loadVolume("validation_08_simulation_sinogram.raw", "float32", size(sinogram_80keV, 1), size(sinogram_80keV, 2), size(sinogram_80keV, 3));




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

simulation_reconstructed_ct_volume = reconstructVolume(number_of_processes, simulation_sinogram, angle_span);
simulation_reconstructed_ct_volume = cell2mat(simulation_reconstructed_ct_volume);
writeVolume("validation_08_simulation_reconstructed_phantom.raw", "float32", simulation_reconstructed_ct_volume);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Sinogram error
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
sinogram_error = abs(sinogram_80keV - simulation_sinogram);
writeVolume("validation_08_sinogram_error.raw", "float32", sinogram_error);
mean(mean(mean(sinogram_error)))
min(min(min(sinogram_error)))
max(max(max(sinogram_error)))



ans =  0.14842
ans =    1.7102e-06
ans =  43.929



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Reconstruction error
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

reconstruction_error = abs(ct_volume_in_mu_80keV - simulation_reconstructed_ct_volume);
writeVolume("validation_08_reconstruction_error.raw", "float32", reconstruction_error);
mean(mean(mean(reconstruction_error)))
min(min(min(reconstruction_error)))
max(max(max(reconstruction_error)))

octave:2> mean(mean(mean(reconstruction_error)))
ans =  0.0018440
octave:3> min(min(min(reconstruction_error)))
ans =    7.6048e-09
octave:4> max(max(max(reconstruction_error)))
ans =  0.15140

