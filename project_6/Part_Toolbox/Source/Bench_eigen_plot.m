% % Visualize information from the eigenspectrum of the graph Laplacian
% %
% % D.P & O.S for the "HPC Course" at USI and
% %                   "HPC Lab for CSE" at ETH Zurich

% % add necessary paths
% addpaths_GP;

% % Graphical output at bisection level
% picture = 0;

% % Cases under consideration
% % load airfoil1.mat;
% % load 3elt.mat;
% % load barth4.mat;
% % load mesh3e1.mat;
% load crack.mat;

% % Initialize the cases
% W      = Problem.A;
% coords = Problem.aux.coord;

% % Steps
% % 1. Construct the graph Laplacian of the graph in question.
% % 2. Compute eigenvectors associated with the smallest eigenvalues.
% % 3. Perform spectral bisection.
% % 4. Visualize:
% %   i.   The first and second eigenvectors.
% %   ii.  The second eigenvector projected on the coordinate system space of graphs.
% %   iii. The spectral bi-partitioning results using the spectral coordinates of each graph.

clc; clear; close all;

% add necessary paths
try
    run('addpaths_GP.m');
catch
    warning('addpaths_GP.m non trovato. Esegui dalla cartella Source.');
end

% Directory dataset
mesh_dir = '../Datasets/2d_meshes/';

%% ---------------------------------------------------------
%% 1. Analisi Autovettori su 'airfoil1' (Task 2.6.1)
%% ---------------------------------------------------------
fprintf('--- PART 1: Analisi Autovettori airfoil1 ---\n');

% Caricamento dati
data = load([mesh_dir 'airfoil1.mat']);
A = data.Problem.A;
xy = data.Problem.aux.coord;

% Calcolo primi 2 autovettori (v1, v2)
% v1 corrisponde a lambda ~ 0
[L, vals, V] = compute_laplacian_eigs(A, 2);
v1 = V(:,1); 
v2 = V(:,2); 

% Plot richiesto dal punto 1
figure('Name', 'Task 2.6.1 - Airfoil Eigenvectors');
subplot(2,1,1);
plot(v1, 'LineWidth', 1.5);
title(['1° Autovettore (v_1). \lambda_1 \approx ' num2str(vals(1), '%.2e')]);
ylabel('Valore'); xlabel('Indice Nodo'); grid on;

subplot(2,1,2);
plot(v2, 'LineWidth', 1.5);
title(['2° Autovettore (v_2 Fiedler). \lambda_2 \approx ' num2str(vals(2), '%.4f')]);
ylabel('Valore'); xlabel('Indice Nodo'); grid on;


%% ---------------------------------------------------------
%% 2. & 3. Proiezioni 3D e Spectral Drawing (Task 2.6.2 & 2.6.3)
%% ---------------------------------------------------------
% Lista grafi richiesti: mesh3e1, barth4, 3elt, crack
graphs_list = {'mesh3e1', 'barth4', '3elt', 'crack'};

fprintf('\n--- PART 2 & 3: Visualizzazione Grafi ---\n');

for i = 1:length(graphs_list)
    gname = graphs_list{i};
    fprintf('Elaborazione: %s...\n', gname);
    
    % Caricamento robusto
    try
        data = load([mesh_dir gname '.mat']);
        if isfield(data, 'Problem')
            A = data.Problem.A;
            % Alcuni file hanno coords in Problem.aux.coord, altri in xy o coords
            if isfield(data.Problem, 'aux'), xy_spatial = data.Problem.aux.coord;
            else, xy_spatial = data.xy; end
        else
            A = data.A;
            xy_spatial = data.coords; 
        end
    catch
        warning(['File non trovato: ' gname]); continue;
    end
    
    % Calcolo primi 3 autovettori (v1, v2, v3)
    % Ci serve v3 per lo Spectral Drawing (Punto 3)
    [L, vals, V] = compute_laplacian_eigs(A, 3);
    v2 = V(:,2);
    v3 = V(:,3);
    
    % Calcolo partizione spettrale (per colorare i due gruppi)
    threshold = median(v2);
    part1 = find(v2 < threshold);
    
    % --- CREAZIONE FIGURA COMBINATA ---
    figure('Name', ['Task 2.6 - ' gname], 'Position', [50, 50, 1200, 500]);
    
    % [SINISTRA] Punto 2: Proiezione Fiedler su Coordinate Spaziali (3D)
    subplot(1, 2, 1);
    % Usiamo scatter3: X,Y fisiche, Z = v2. Colore = v2
    scatter3(xy_spatial(:,1), xy_spatial(:,2), v2, 12, v2, 'filled');
    colormap('jet'); colorbar;
    title(['Fiedler Vector (v_2) su Coordinate Fisiche: ' gname]);
    xlabel('x'); ylabel('y'); zlabel('v_2 value');
    view(-30, 30); axis equal;
    
    % [DESTRA] Punto 3: Spectral Drawing (x=v2, y=v3)
    subplot(1, 2, 2);
    xy_spectral = [v2, v3];
    
    % Usiamo gplotpart per visualizzare il taglio nel dominio spettrale
    try
        gplotpart(A, xy_spectral, part1);
        title(['Spectral Layout (x=v_2, y=v_3): ' gname]);
        xlabel('v_2'); ylabel('v_3');
        axis tight;
    catch
        % Fallback se gplotpart fallisce
        scatter(v2, v3, 10, 'k', 'filled');
    end
end

fprintf('\nCalcolo completato. Figure generate.\n');


%% --- FUNZIONE LOCALE PER IL CALCOLO (Da copiare in fondo al file) ---
function [L, vals, V] = compute_laplacian_eigs(A, k)
    n = size(A,1);
    
    % 1. Construct Graph Laplacian
    degrees = sum(A, 2);
    D = spdiags(degrees, 0, n, n);
    L = D - A;
    L = (L + L') / 2; % Assicura simmetria numerica
    
    % 2. Compute Eigenvectors (Smallest Magnitude)
    % Usiamo 'sm' (Smallest Magnitude) con un sigma piccolo o 0
    opts.issym = 1; 
    opts.isreal = 1;
    opts.tol = 1e-6;
    
    % Calcoliamo k autovettori
    try
        [V, D_val] = eigs(L, k, 'sm', opts);
    catch
        % Fallback per matrici piccole o problematiche
        [V, D_val] = eigs(L, k, 0, opts);
    end
    
    % Ordina per autovalori crescenti (lambda1, lambda2...)
    vals = diag(D_val);
    [vals, idx] = sort(vals);
    V = V(:, idx);
end