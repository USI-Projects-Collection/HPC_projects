function [part1,part2] = bisection_spectral(A,xy,picture)
% bisection_spectral : Spectral partition of a graph.
%
% D.P & O.S for the "HPC Course" at USI and
%                   "HPC Lab for CSE" at ETH Zuric
%
% [part1,part2] = bisection_spectral(A) returns a partition of the n vertices
%                 of A into two lists part1 and part2 according to the
%                 spectral bisection algorithm of Simon et al:
%                 Label the vertices with the components of the Fiedler vector
%                 (the second eigenvector of the Laplacian matrix) and partition
%                 them around the median value or 0.

% Require: G(V,E),
% Ensure: A bisection of G
% 1: function SPECTRALBISECTION(graph G(V,E))
% 2: Form the graph Laplacian matrix L
% 3: Calculate the second smallest eigenvalue λ(2) and its associated eigenvector u(2)
% 4: Set 0 or the median of all components of u(2) as threshold ϵ.
% 5: ChooseV1 := {vi ∈V|ui <ϵ}, V2 := {{vi ∈V|ui ≥ϵ}.
% 6: return V1,V2 
% 7: end function

% disp(' ');
% disp(' HPC Lab at USI:   ');
% disp(' Implement spectral bisection');
% disp(' ');


% Steps
% 1. Construct the Laplacian.
% L = D - A
d = sum(A,2);
D = diag(d);
L = D - A;

% 2. Calculate its eigensdecomposition.
% Calculate the second smallest eigenvalue λ(2) and its associated eigenvector u(2)
[V, ~] = eigs(L, 2, 'smallestreal');

% Second eigenvector = Fiedler vector
fiedler = V(:,2);

% 3. Label the vertices with the components of the Fiedler vector.
% Set 0 or the median of all components of u(2) as threshold ϵ.
threshold = median(fiedler);

% 4. Partition them around their median value, or 0.
% ChooseV1 := {vi ∈V|ui <ϵ}, V2 := {{vi ∈V|ui ≥ϵ}.
part1 = find(fiedler < threshold);
part2 = find(fiedler >= threshold);

if picture == 1
    gplotpart(A,xy,part1);
    title('Spectral Bisection using the Fiedler Vector');

end

% <<<< Dummy implementation to generate a partitioning
% n = size(A,1);
% map = zeros(n,1);
% map(1:round((n/2))) = 0; 
% map((round((n/2))+1):n) = 1;
% [part1,part2] = other(map);

% if picture == 1
%     gplotpart(A,xy,part1);
%     title('Spectral bisection (dummy) using the Fiedler Eigenvector');

% Dummy implementation to generate a partitioning >>>>
% end
