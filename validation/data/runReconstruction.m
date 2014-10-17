%!/usr/bin/octave

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the image package (needed for FBP)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pkg load image




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function volume=loadVolume(aFileNameWithExtension,
                           aWidth,
                           aHeight,
                           aDepth)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% Allocate the memory
    volume = zeros([aWidth aHeight aDepth]);
    slice = zeros([aWidth aHeight]);

	% Open the file
    file_descriptor = fopen(aFileNameWithExtension, "rb");

	% Read all the slices
	for slice_id = 1:aDepth

	    % Read the sinogram
		slice = fread(file_descriptor, [aWidth, aHeight], "float32");

        % Get the corresponding slice
        volume(:, :, slice_id) = slice;
    endfor

    % Close the file
    fclose(file_descriptor);

	% Return the volume
    return;

endfunction




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function writeVolume(aFileNameWithExtension,
                     aVolume)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	% Open the file
    file_descriptor = fopen(aFileNameWithExtension, "wb");

	% Write all the slices
    fwrite(file_descriptor, aVolume, "float32");

    % Close the file
    fclose(file_descriptor);

    size(aVolume)
    return;

endfunction




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

width_in_number_of_pixels = 301;
pixel_size_in_mm          =   0.3;
number_of_angles          = 360;
first_angle               =   0.0;
angle_offset              =   0.5;
angle_span                = first_angle:angle_offset:((number_of_angles - 1) * angle_offset);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Load the data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sinogram = loadVolume("sinogram.raw", width_in_number_of_pixels, number_of_angles, 1);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Apply FBP
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ct_volume_in_mu_80keV = iradon(sinogram, angle_span, 'Spline', 'Hamming');




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Normalise data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

ratio = width_in_number_of_pixels / (width_in_number_of_pixels * pixel_size_in_mm / 10);
ct_volume_in_mu_80keV *= ratio;
writeVolume("ct_volume_in_mu_80keV.raw", ct_volume_in_mu_80keV);




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract soft tissue ROI and print the corresponding attenuation coefficient
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sub_matrix_soft_tissue = ct_volume_in_mu_80keV(60:60+9,60:60+9);
mean(mean(sub_matrix_soft_tissue))




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Extract bone ROI and print the corresponding attenuation coefficient
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

sub_matrix_bone = ct_volume_in_mu_80keV(95:95+9,95:95+9);
mean(mean(sub_matrix_bone))
