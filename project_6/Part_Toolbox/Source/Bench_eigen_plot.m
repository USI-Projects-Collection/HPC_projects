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

try
    run('addpaths_GP.m');
catch
    warning('addpaths_GP.m non trovato.');
end

% Directory dataset
mesh_dir = '../Datasets/2d_meshes/';

% Creazione cartella per salvare i risultati
output_dir = 'Risultati_Task2_6';
if ~exist(output_dir, 'dir')
    mkdir(output_dir);
end

%% --- PUNTO 1: Analisi Autovettori su 'airfoil1' ---
fprintf('--- Elaborazione airfoil1 (Eigenvectors) ---\n');

data = load([mesh_dir 'airfoil1.mat']);
if isfield(data, 'Problem'), A = data.Problem.A; else, A = data.A; end

[L, vals, V] = compute_laplacian_eigs(A, 2);
v1 = V(:,1); 
v2 = V(:,2); 

f1 = figure('Name', 'Airfoil Eigenvectors', 'Visible', 'on'); % 'off' per non vederle apparire
subplot(2,1,1);
plot(v1, 'LineWidth', 1.5);
title(['1° Autovettore (v_1). \lambda_1 \approx ' num2str(vals(1), '%.2e')]);
grid on; axis tight;

subplot(2,1,2);
plot(v2, 'LineWidth', 1.5);
title(['2° Autovettore (Fiedler). \lambda_2 \approx ' num2str(vals(2), '%.4f')]);
grid on; axis tight;

% Salva e chiudi
saveas(f1, fullfile(output_dir, '1_Airfoil_Eigenvectors.png'));
fprintf(' -> Salvato 1_Airfoil_Eigenvectors.png\n');
% close(f1); % Decommenta se vuoi chiuderla subito


%% --- PUNTO 2 & 3: Proiezioni e Spectral Drawing ---
graphs_list = {'mesh3e1', 'barth4', '3elt', 'crack'};

fprintf('\n--- Elaborazione Grafi (3D e Spectral) ---\n');

for i = 1:length(graphs_list)
    gname = graphs_list{i};
    fprintf('Grafo: %s...\n', gname);
    
    % Caricamento
    try
        data = load([mesh_dir gname '.mat']);
        if isfield(data, 'Problem')
            A = data.Problem.A;
            if isfield(data.Problem, 'aux'), xy = data.Problem.aux.coord; else, xy = data.xy; end
        else
            A = data.A; xy = data.coords;
        end
    catch
        warning(['Saltato ' gname]); continue;
    end
    
    % Calcolo Autovettori (v2, v3)
    [L, vals, V] = compute_laplacian_eigs(A, 3);
    v2 = V(:,2);
    v3 = V(:,3);
    
    % Calcolo partizione (per i colori)
    threshold = median(v2);
    part1 = find(v2 < threshold);
    
    % --- FIGURA A: Proiezione 3D (Coordinate Fisiche + Colore Fiedler) ---
    fA = figure('Name', [gname ' Physical'], 'Visible', 'on');
    scatter3(xy(:,1), xy(:,2), v2, 12, v2, 'filled');
    colormap('jet'); colorbar;
    title(['Physical Coords + Fiedler (Z): ' gname]);
    view(-30, 30); axis equal;
    saveas(fA, fullfile(output_dir, [gname '_Physical_3D.png']));
    fprintf(' -> Salvato %s_Physical_3D.png\n', gname);
    % close(fA); 
    
    % --- FIGURA B: Spectral Drawing (Coordinate v2, v3) ---
    fB = figure('Name', [gname ' Spectral'], 'Visible', 'on');
    xy_spectral = [v2, v3];
    
    % !! Attenzione: cancella la figura corrente, quindi deve stare da sola
    try
        gplotpart(A, xy_spectral, part1);
        title(['Spectral Layout (v_2, v_3): ' gname]);
        axis tight;
        saveas(fB, fullfile(output_dir, [gname '_Spectral.png']));
        fprintf(' -> Salvato %s_Spectral.png\n', gname);
    catch
        warning('Errore gplotpart');
    end
    % close(fB);
end


%% --- FUNZIONE CALCOLO ---
function [L, vals, V] = compute_laplacian_eigs(A, k)
    n = size(A,1);
    degrees = sum(A, 2);
    D = spdiags(degrees, 0, n, n);
    L = D - A;
    L = (L + L') / 2; 
    
    opts.issym = 1; 
    opts.isreal = 1;
    opts.tol = 1e-5; % Tolleranza un po' più lasca per velocità
    
    try
        [V, D_val] = eigs(L, k, 'sm', opts);
    catch
        [V, D_val] = eigs(L, k, 0, opts);
    end
    
    vals = diag(D_val);
    [vals, idx] = sort(vals);
    V = V(:, idx);
end