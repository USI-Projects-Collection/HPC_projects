function [cut_recursive,cut_kway] = Bench_metis(picture)
% Compare recursive bisection and direct k-way partitioning,
% as implemented in the Metis 5.0.2 library.

% Default argument (matlab equivalent of picture=0 for python)
if nargin < 1
    picture = 0;
end
%  Add necessary paths
addpaths_GP;

% List of countries / datasets
Graphs = {};

% Graphs in question
% load usroads;
% load luxembourg_osm;
load('../Datasets/Roads/usroads.mat');  
Graphs{end+1}.A  = Problem.A;
Graphs{end}.xy   = Problem.aux.coord;
Graphs{end}.name = 'usroads';

load('../Datasets/Roads/luxembourg_osm.mat');
Graphs{end+1}.A  = Problem.A;
Graphs{end}.xy   = Problem.aux.coord;
Graphs{end}.name = 'luxembourg';


% Steps
% 1. Initialize the cases
mat_files = {'GR_graph.mat','Swiss_graph.mat','VN_graph.mat','NO_graph.mat','RU_graph.mat'};
for i = 1:length(mat_files)
    load(fullfile('../Datasets/Countries_Meshes/mat', mat_files{i}));
    Graphs{end+1}.A  = W;
    Graphs{end}.xy   = coords;
    [~,name,~] = fileparts(mat_files{i});
    Graphs{end}.name = name;
end

% Partition sizes
ks = [16 32];
nGraphs = length(Graphs);
cut_recursive = zeros(nGraphs, length(ks));
cut_kway      = zeros(nGraphs, length(ks)); % 2 graphs × 2 k-values

% store the resulting kway maps for plotting
map_kway_all  = cell(nGraphs, length(ks));

% 2. Call metismex to
%     a) Recursively partition the graphs in 16 and 32 subsets.
for g = 1:nGraphs
    A  = Graphs{g}.A;
    xy = Graphs{g}.xy;

    for i = 1:length(ks) % -> for k=16 and k=32
        k = ks(i);

        % a. Recursive partitioning
        map_rec = metismex('PartGraphRecursive', A, k);
        cut_recursive(g,i) = cutsize(A, map_rec);

        %     b) Perform direct k-way partitioning of the graphs in 16 and 32 subsets.
        map_kway = metismex('PartGraphKway', A, k);
        cut_kway(g,i) = cutsize(A, map_kway);

        % store for plotting
        map_kway_all{g,i} = map_kway;

        % print
        fprintf('[%s] k=%d | recursive cut=%d | kway cut=%d\n', ...
            Graphs{g}.name, k, cut_recursive(g,i), cut_kway(g,i));
    end
end

% 3. Visualize the results for 32 partitions.
if picture == 1
    for g = 1:nGraphs
        if ismember(Graphs{g}.name, {'usroads','luxembourg','RU_graph'})
            figure;
                gplotmap(Graphs{g}.A, Graphs{g}.xy, map_kway_all{g,2});
            title(sprintf('%s – 32-way Kway METIS', Graphs{g}.name));
        end
    end
end
end
