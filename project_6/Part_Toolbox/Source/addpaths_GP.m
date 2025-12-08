% add necessary paths for the Partitioning Toolbox

script_dir = fileparts(mfilename('fullpath')); % current path
base_dir = fullfile(script_dir, '..');

addpath(fullfile(base_dir, 'Datasets'));
addpath(fullfile(base_dir, 'Datasets', 'Mesh_generation'));
addpath(fullfile(base_dir, 'Datasets', '2d_meshes'));
addpath(fullfile(base_dir, 'Datasets', 'Roads'));
addpath(fullfile(base_dir, 'Datasets', 'Countries_Meshes', 'csv'));
addpath(fullfile(base_dir, 'Datasets', 'Countries_Meshes', 'mat'));
addpath(fullfile(base_dir, 'External'));
addpath(fullfile(base_dir, 'Source', 'Visualization'));
