% Script to load .csv lists of adjacency matrices and the corresponding 
% coordinates. 
% The resulting graphs should be visualized and saved in a .csv file.
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

addpaths_GP;
% Steps
% 0. Define the graphs to load
countries = {'VN', 'NO'};
nodes_list = [4031, 9935];
for i = 1:length(countries)
    country = countries{i};
    nodes = nodes_list(i);

    % 1. Load the .csv files
    %help csvread
    %pause;
    adj_file = sprintf('%s-%d-adj.csv', country, nodes);
    coords_file = sprintf('%s-%d-pts.csv', country, nodes);

    edge_list = readmatrix(adj_file, "NumHeaderLines", 1);
    coords = readmatrix(coords_file, "NumHeaderLines", 1);

    % 2. Construct the adjaceny matrix (NxN). There are multiple ways
    %    to do so.
    %help accumarray
    %help sparse
    %pause;
    W = sparse(edge_list(:,1), edge_list(:,2), 1, nodes, nodes);

    if ~issymmetric(W)
        fprintf('Adjacency matrix is not symmetric. Fixing...\n');
        W = (W + W') / 2;   % Symmetrize
    end

    % 3. Visualize the resulting graphs
    % help gplotg
    % pause;
    figure;
    gplotg(W, coords);
    title(sprintf('Graph of %s', country));

    % 4. Save the resulting graphs
    %help save
    %pause;
    save_dir = fullfile(script_dir, '..', 'Datasets', 'Countries_Meshes', 'mat');

    mat_filename = sprintf('%s_graph.mat', country);
    save(fullfile(save_dir, mat_filename), 'W', 'coords');
    fprintf('Saved graph to %s\n', fullfile(save_dir, mat_filename));

end

% Example of the desired graph format for CH

%load Swiss_graph.mat
%W      = CH_adj;
%coords = CH_coords;   
%whos

%figure;
%gplotg(W,coords);
