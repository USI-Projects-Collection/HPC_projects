% Visualize information from the eigenspectrum of the graph Laplacian
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zurich

% add necessary paths
addpaths_GP;

% Graphical output at bisection level
picture = 0;

% Cases under consideration
load airfoil1.mat;
% load 3elt.mat;
% load barth4.mat;
% load mesh3e1.mat;
load crack.mat;

% Initialize the cases
W      = Problem.A;
coords = Problem.aux.coord;

% Steps
% 1. Construct the graph Laplacian of the graph in question.
% 2. Compute eigenvectors associated with the smallest eigenvalues.
% 3. Perform spectral bisection.
% 4. Visualize:
%   i.   The first and second eigenvectors.
%   ii.  The second eigenvector projected on the coordinate system space of graphs.
%   iii. The spectral bi-partitioning results using the spectral coordinates of each graph.

%% ---------------------------
%% Implementation (minimal changes; uses existing helper functions)
%% ---------------------------

% Helper to compute Laplacian and first few eigenvectors (v1,v2,v3)
function [L, vals, vecs] = compute_laplacian_eigs(A, nev)
    if nargin < 2, nev = 3; end
    d = sum(A,2);
    D = spdiags(d,0,size(A,1),size(A,1));
    L = D - A;
    % ensure symmetric/real
    L = (L + L')/2;
    % Use eigs to compute the smallest eigenvalues (including 0)
    opts.isreal = true; opts.issym = true;
    try
        [vecs, valsm] = eigs(L, nev, 'smallestabs', opts);
        vals = diag(valsm);
    catch
        % fallback to full eig for small graphs / if eigs fails
        [Vfull, Dfull] = eig(full(L));
        [vals_sorted, idx] = sort(diag(Dfull));
        vecs = Vfull(:, idx(1:nev));
        vals = vals_sorted(1:nev);
    end
end

%% ---------------------------
%% Part 1: Airfoil1 eigenvectors
%% ---------------------------

fprintf('--- Part 1: Airfoil1 eigenvectors ---\n');

% load('airfoil1.mat');  % deve essere disponibile nel path

% W_air = Problem.A;
% coords_air = Problem.aux.coord;
% Compute eigenvectors
[L_air, vals_air, vecs_air] = compute_laplacian_eigs(W_air, 2);
v1_air = vecs_air(:,1);
v2_air = vecs_air(:,2);
% v3 = [];
% if size(vecs_crack,2) >= 3
%     v3 = vecs_crack(:,3);
% end

% Plot first eigenvector (λ1)
figure;
% subplot(2,2,1);
plot(v1, '.-');
xlabel('node index'); ylabel('v_1 entries');
title('First eigenvector (smallest \lambda_1) entries');

% subplot(2,2,2);
plot(v2, '.-');
xlabel('node index'); ylabel('v_2 entries');
title('Second eigenvector (Fiedler) entries');

% subplot(2,2,[3 4]);
% show v2 projected on coordinates as 3D scatter (z = v2)
% if exist('coords','var') && size(coords,2) >= 2
scatter3(coords(:,1), coords(:,2), v2, 8, v2, 'filled');
xlabel('x'); ylabel('y'); zlabel('v_2 value');
title('Fiedler values projected on coordinate space (z = v_2)');
colorbar;
view(45,30);
axis tight;
% else
%     % fallback 2D colored scatter
%     scatter(1:length(v2), v2, 8, v2, 'filled');
%     xlabel('node index'); ylabel('v_2 value');
%     title('Fiedler values (no coordinates available)');
% end

% Comment: typical behavior: v1 is near-constant, v2 shows structure / sign changes.
% if numel(vals_crack) >= 3
%     fprintf('Eigenvalues (smallest three) for crack.mat: %g, %g, %g\n', vals_crack(1), vals_crack(2), vals_crack(3));
% else
%     fprintf('Eigenvalues for crack.mat: %s\n', mat2str(vals_crack));
% end

%% Part 2: compute and project v2 for several graphs (mesh3e1, barth4, 3elt, crack)
% Explicit loads as requested (minimal changes)
% load mesh3e1.mat;
% load barth4.mat;
% load 3elt.mat;
% load crack.mat;

% graphs_to_process = {'mesh3e1','barth4','3elt','crack'};
% n_graphs = numel(graphs_to_process);

% figure;
% for idx = 1:n_graphs
%     name = graphs_to_process{idx};

%     % explicit load (will error if file missing - consistent with your explicit-load request)
%     load([name '.mat']);

%     % extract adjacency and coords in the standard, robust way
%     if exist('Problem','var') && isfield(Problem,'A')
%         A = Problem.A;
%         if isfield(Problem,'aux') && isfield(Problem.aux,'coord')
%             coords_this = Problem.aux.coord;
%         elseif isfield(Problem,'coords')
%             coords_this = Problem.coords;
%         elseif isfield(Problem,'xy')
%             coords_this = Problem.xy;
%         else
%             coords_this = [];
%         end
%     elseif exist('W','var') && exist('coords','var')
%         A = W; coords_this = coords;
%     else
%         clear Problem W coords;
%         error('%s.mat does not contain expected variables.', name);
%     end

%     % compute Fiedler (v2)
%     [~, ~, vecs_tmp] = compute_laplacian_eigs(A, 3);
%     v2_tmp = vecs_tmp(:,2);

%     % plot into a 2x2 grid
%     subplot(2,2,idx);
%     if ~isempty(coords_this) && size(coords_this,2) >= 2
%         scatter(coords_this(:,1), coords_this(:,2), 8, v2_tmp, 'filled');
%         title(sprintf('%s: Fiedler (v_2) projected on coords', name), 'Interpreter', 'none');
%         axis equal off; colorbar;
%     else
%         plot(v2_tmp, '.-');
%         title(sprintf('%s: v_2 entries (no coords)', name), 'Interpreter', 'none');
%     end

%     % clean up so next iteration loads fresh variables
%     clear Problem W coords A coords_this vecs_tmp v2_tmp;
% end

% %% Part 3: spectral layout using (v2,v3) and spectral bipartitioning
% % For each graph draw original partition (spatial) and spectral layout (v2,v3)
% % load mesh3e1.mat;
% % load barth4.mat;
% % load 3elt.mat;
% % load crack.mat;

% graphs_spec = {'mesh3e1','barth4','3elt','crack'};
% for idx = 1:numel(graphs_spec)
%     name = graphs_spec{idx};
%     load([name '.mat']);

%     % extract adjacency and coords
%     if exist('Problem','var') && isfield(Problem,'A')
%         A = Problem.A;
%         if isfield(Problem,'aux') && isfield(Problem.aux,'coord')
%             coords_this = Problem.aux.coord;
%         elseif isfield(Problem,'coords')
%             coords_this = Problem.coords;
%         elseif isfield(Problem,'xy')
%             coords_this = Problem.xy;
%         else
%             coords_this = [];
%         end
%     elseif exist('W','var') && exist('coords','var')
%         A = W; coords_this = coords;
%     else
%         clear Problem W coords;
%         error('%s.mat does not contain expected variables.', name);
%     end

%     % compute v2 and v3
%     [~, ~, vecs_tmp] = compute_laplacian_eigs(A, 4);
%     v2_tmp = vecs_tmp(:,2);
%     if size(vecs_tmp,2) >= 3
%         v3_tmp = vecs_tmp(:,3);
%     else
%         v3_tmp = zeros(size(v2_tmp));
%     end

%     % spectral partition by median
%     thresh = median(v2_tmp);
%     map_spec = double(v2_tmp >= thresh);
%     part1 = find(map_spec == 0);

%     figure('Name', sprintf('%s: spatial vs spectral', name), 'NumberTitle','off');
%     subplot(1,2,1);
%     if ~isempty(coords_this)
%         try
%             gplotpart(A, coords_this, part1);
%         catch
%             scatter(coords_this(:,1), coords_this(:,2), 8, map_spec, 'filled');
%             axis equal off;
%         end
%         title(sprintf('%s spatial partition (from v_2)', name), 'Interpreter', 'none');
%     else
%         plot(map_spec);
%         title(sprintf('%s spatial partition (no coords)', name), 'Interpreter', 'none');
%     end

%     subplot(1,2,2);
%     scatter(v2_tmp, v3_tmp, 8, map_spec, 'filled');
%     axis equal tight;
%     xlabel('v_2'); ylabel('v_3');
%     title(sprintf('%s spectral layout (v_2, v_3)', name), 'Interpreter', 'none');

%     clear Problem W coords A coords_this vecs_tmp v2_tmp v3_tmp map_spec thresh part1;
% end
