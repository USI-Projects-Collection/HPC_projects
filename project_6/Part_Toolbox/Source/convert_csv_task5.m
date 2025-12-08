function convert_csv_to_mat()
% Convert CSV country graph data into MAT files for METIS benchmarking
% Countries: Greece, Norway, Russia, Switzerland, Vietnam

addpaths_GP;
% Steps
% 0. Define the graphs to load
countries = {'GR','NO','RU','VN'};
nodes_list = [3117, 9935, 40527, 4031];

script_dir = fileparts(mfilename('fullpath'));  % current folder
save_dir = fullfile(script_dir, '..', 'Datasets', 'Countries_Meshes', 'mat');
if ~exist(save_dir, 'dir')
    mkdir(save_dir);
end

for i = 1:length(countries)
    country = countries{i};
    nodes = nodes_list(i);

    % 1. Load the .csv files
    %help csvread
    %pause;

    adj_file = fullfile('Datasets','Countries_Meshes','csv', sprintf('%s-%d-adj.csv', country, nodes));
    coords_file = fullfile('Datasets','Countries_Meshes','csv', sprintf('%s-%d-pts.csv', country, nodes));

    edge_list = readmatrix(adj_file, "NumHeaderLines",1);
    coords = readmatrix(coords_file, "NumHeaderLines",1);

    % 2. Construct the adjaceny matrix (NxN). There are multiple ways
    %    to do so.
    %help accumarray
    %help sparse
    %pause;
    W = sparse(edge_list(:,1), edge_list(:,2), 1, nodes, nodes);

    % Ensure symmetry
    if ~issymmetric(W)
        fprintf('%s: adjacency matrix is not symmetric. Fixing...\n', country);
        W = (W + W') / 2;   % Symmetrize
    end

    % 4. Save the resulting graphs
    %help save
    %pause;
    mat_filename = fullfile(save_dir, sprintf('%s_graph.mat', country));
    save(mat_filename, 'W', 'coords');
    fprintf('Saved %s\n', mat_filename);
end
end
