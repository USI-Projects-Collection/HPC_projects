% Benchmark for recursively partitioning meshes, based on various
% bisection approaches
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich



% add necessary paths
addpaths_GP;
nlevels_a = 3;
nlevels_b = 4;

fprintf('       *********************************************\n')
fprintf('       ***  Recursive graph bisection benchmark  ***\n');
fprintf('       *********************************************\n')

% load cases
cases = {
    'airfoil1.mat';
    'netz4504_dual.mat';
    'stufe.mat';
    '3elt.mat';
    'barth4.mat';
    'ukerbe1.mat';
    'crack.mat';
    };

nc = length(cases);
maxlen = 0;
for c = 1:nc
    if length(cases{c}) > maxlen
        maxlen = length(cases{c});
    end
end

for c = 1:nc
    fprintf('.');
    sparse_matrices(c) = load(cases{c});
end


fprintf('\n\n Report Cases         Nodes     Edges\n');
fprintf(repmat('-', 1, 40));
fprintf('\n');
for c = 1:nc
    spacers  = repmat('.', 1, maxlen+3-length(cases{c}));
    [params] = Initialize_case(sparse_matrices(c));
    fprintf('%s %s %10d %10d\n', cases{c}, spacers,params.numberOfVertices,params.numberOfEdges);
end

%% Create results table
fprintf('\n%7s %16s %20s %16s %16s\n','Bisection','Spectral','Metis 5.0.2','Coordinate','Inertial');
fprintf('%10s %10d %6d %10d %6d %10d %6d %10d %6d\n','Partitions',8,16,8,16,8,16,8,16);
fprintf(repmat('-', 1, 100));
fprintf('\n');


for c = 1:nc
    spacers = repmat('.', 1, maxlen+3-length(cases{c}));
    fprintf('%s %s', cases{c}, spacers);
    sparse_matrix = load(cases{c});
    

    % Recursively bisect the loaded graphs in 8 and 16 subgraphs.
    % Steps
    % 1. Initialize the problem
    [params] = Initialize_case(sparse_matrices(c));
    W      = params.Adj;
    coords = params.coords;
    % 2. Recursive routines
    % i. Spectral    
    [map_spec_8, sep_spec_8] = rec_bisection('bisection_spectral', nlevels_a, W, coords, 0);
    [map_spec_16, sep_spec_16] = rec_bisection('bisection_spectral', nlevels_b, W, coords, 0);
    % ii. Metis
    [map_metis_8, sep_metis_8] = rec_bisection('bisection_metis', nlevels_a, W, coords, 0);
    [map_metis_16, sep_metis_16] = rec_bisection('bisection_metis', nlevels_b, W, coords, 0);
    % iii. Coordinate    
    [map_coord_8, sep_coord_8] = rec_bisection('bisection_coordinate', nlevels_a, W, coords, 0);
    [map_coord_16, sep_coord_16] = rec_bisection('bisection_coordinate', nlevels_b, W, coords, 0);
    % iv. Inertial
    [map_inertial_8, sep_inertial_8] = rec_bisection('bisection_inertial', nlevels_a, W, coords, 0);
    [map_inertial_16, sep_inertial_16] = rec_bisection('bisection_inertial', nlevels_b, W, coords, 0);

    % 3. Calculate number of cut edges
    run_spec_8 = size(sep_spec_8, 1);
    run_spec_16 = size(sep_spec_16, 1);
    run_metis_8 = size(sep_metis_8, 1);
    run_metis_16 = size(sep_metis_16, 1);
    run_coord_8 = size(sep_coord_8, 1);
    run_coord_16 = size(sep_coord_16, 1);
    run_inertial_8 = size(sep_inertial_8, 1);
    run_inertial_16 = size(sep_inertial_16, 1);

    % 4. Visualize the partitioning result
    if strcmp(cases{c}, 'crack.mat')
        
        %  Spectral 
        figure;
        gplotmap(W, coords, map_spec_16);
        title('Spectral Partitioning of crack.mat, 16 parts');
    
        %  METIS recursive bisection 
        figure;
        gplotmap(W, coords, map_metis_16);
        title('METIS Recursive Partitioning of crack.mat, 16 parts');
    
        %  Coordinate bisection 
        figure;
        gplotmap(W, coords, map_coord_16);
        title('Coordinate Partitioning of crack.mat, 16 parts');
    
        %  Inertial bisection 
        figure;
        gplotmap(W, coords, map_inertial_16);
        title('Inertial Partitioning of crack.mat, 16 parts');
    
    end

    
    fprintf('%6d %6d %10d %6d %10d %6d %10d %6d\n', ...
            run_spec_8, run_spec_16, ...
            run_metis_8, run_metis_16, ...
            run_coord_8, run_coord_16, ...
            run_inertial_8, run_inertial_16);
    
    % fprintf('%6d %6d %10d %6d %10d %6d %10d %6d\n',0,0,...
    % 0,0,0,0,0,0);
    
end
