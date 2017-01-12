function f = Gauss( x, mu, s )
%Gaussian function implementaion

p1 = -.5 * ((x - mu)/s) .^ 2;

% normalize Gaussian
% p2 = (s * sqrt(2*pi));

% do not normalize Gaussian (this way scale factor in front of Gaussian
% sets peak position)
p2 = 1.
f = exp(p1) ./ p2; 

end
