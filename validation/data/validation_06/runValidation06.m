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
% Load the statistics package (needed for std)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

pkg load statistics




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Add the path with octave routines
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

addpath("../octave")




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Parameters
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


close all;

volume_size = [301 301 1];
voxel_size = [1.0 1.0 1.0];
number_of_tests = 999;

gate_simulation_image = zeros(volume_size);

for j = [0:number_of_tests-1]
	directory_name = "gate_simulation";
	file_name = strcat(directory_name, "/run_");
	file_name = strcat(file_name, int2str(j));
	file_name = strcat(file_name, "/benchmarkVRTCT_000.dat");
		
    % Check if the file exists
    if  exist (file_name, "file")
   	    gate_simulation_image += loadVolume(file_name, "float32", volume_size(1), volume_size(2), volume_size(3));
    end
end

writeVolume("gate_simulation.raw", "float32", gate_simulation_image);


mean_gate = mean(gate_simulation_image(:));
std_gate  = std(gate_simulation_image(:));
gate_norm =  (gate_simulation_image - mean_gate) / std_gate;

writeVolume("gate_simulation_normalised.raw", "float32", gate_norm);

gpu_simulation_image = loadVolume("single_projection.raw", "float32", volume_size(1), volume_size(2), volume_size(3));

mean_gpu = mean(gpu_simulation_image(:));
std_gpu  = std(gpu_simulation_image(:));
gpu_norm =  (gpu_simulation_image - mean_gpu) / std_gpu;

writeVolume("single_projection_normalised.raw", "float32", gpu_norm);

error_map = 100.0 * abs(abs(gpu_norm - gate_norm) / gate_norm);
writeVolume("error_map.raw", "float32", error_map);

min_value = min(min(gate_norm));
max_value = max(max(gate_norm));

gate_norm = (gate_norm - min_value) / (max_value - min_value);
gpu_norm  = ( gpu_norm - min_value) / (max_value - min_value);

climits = [min(min(gate_norm)), max(max(gate_norm))];
figure;
imagesc(gate_norm', climits);
colormap("rainbow");
colorbar;
daspect( [ 1 1 1 ] );
print("gate_norm.png","-dpng"); 

figure;
imagesc(gpu_norm', climits);
colormap("rainbow");
colorbar;
daspect( [ 1 1 1 ] );
print("gpu_norm.png","-dpng"); 

figure;
imagesc(error_map);
colormap("rainbow");
colorbar;
daspect( [ 1 1 1 ] );
print("error.png","-dpng"); 

gate_profile = gate_norm( : , int32(size(gate_norm, 2) / 2));
gpu_profile  =  gpu_norm( : , int32(size( gpu_norm, 2) / 2));

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) + 1);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) + 1);

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) - 1);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) - 1);

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) + 2);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) + 2);

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) - 2);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) - 2);

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) + 3);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) + 3);

gate_profile += gate_norm( : , int32(size(gate_norm, 2) / 2) - 3);
gpu_profile  +=  gpu_norm( : , int32(size( gpu_norm, 2) / 2) - 3);

gate_profile /= 7;
gpu_profile  /= 7;

figure

x = 1:size(gate_profile, 1);
plot(x, gpu_profile, '-', x, gate_profile, 'ro');

xlabel('Pixel Index');
ylabel('Normalised Energy');
title('Horizontal Profile');
legend('gVirtualXRay','Gate');

save -ascii gate_profile.txt gate_profile;
save -ascii gpu_profile.txt  gpu_profile;

