
% define laboratory coordinate system:
% 'x' goes 'up'
% 'y' goes 'forward'
% 'z' goes 'right'

% All length units in m(eters)
% All magnetic flux units in T(esla)

% select magnetic field map
BMap = 'BGauss5';

% maximum trajectory length for tracking (this is actually 'track length'
% that the particle traverses
LRange = [ 0 10 ];

% cutoff z position- this is absolute z in laboratory frame
MaxZ = 4;

% number of steps in time for tracking
tsteps = 1000;

% particle start conditions
R0 = [ 0 0 0 ]; % vertex

pTotal = 0.01; % GeV
phi = 0;
theta = 0;

% some natural constants / conversion parameters
cSI = 2.99792458e8; % m/s, speed of light

% initial momentum vector
P0   = [ pTotal * sin( theta ) 0 pTotal * cos( theta ) ];% GeV/c

% Define lStep = const; [lStep] = m
% This is the length of one step; bending, change in angle, and change
% in momentum direction are expressed depending on this parameter
lSteps = 500;
lStep = diff(LRange) / lSteps;

% track particle through magnetic field
R = R0; % current position vector
P = P0; % current momentum vector
alpha = 0; % bending angle
kappa = 0; % curvature = 1 / bending radius
deltaYRaw = 0; % total kick in 'y' direction

for tstep = 1:tsteps

    Rs(tstep,:) = R;
    PNorms(tstep) = norm(P);

    Alphas(tstep) = alpha;
    Kappas(tstep) = kappa;
    DeltaYRaws(tstep) = deltaYRaw;
        
    % get B field at current position R
    BLocal = GetLocalB( BMap, R );

    % projection of magnetic field on particel velocity vector
    % Field 'Longitudinal' w.r.t. particle momentum
    BLLocal = dot(P,BLocal) * P / ( P * P' );
    
    % Field 'Transverse' w.r.t. particle momentum
    BTLocal = cross(P,BLocal) / norm(P);

    % get total, longitudinal, and transverse field norm
    b_i  = norm( BLocal  );
    bL_i = norm( BLLocal );
    bT_i = norm( BTLocal );
    if(BTLocal(1,2) < 0)
        bT_i = bT_i * -1;
    end
    
    % save B at current position to vector
    BTotals(tstep) = b_i;
    BTs(tstep) = bT_i;
    BLs(tstep) = bL_i;
    
    % check if tracking has passed ZMax position limit
    if ( R(3) > MaxZ )
        break;
    end

    % new position R
    DR = P / norm(P) * lStep;
    R = R + DR;
    
    %find new momentum
    DP = 0.3 * lStep * cross(P,BLocal) / norm(P);
    P = P + DP;
    
    %force normalization
    pNorm = ( pTotal / norm( P ) );
    P = P .* pNorm;
    
    % bending radius
    radius = pTotal / ( 0.3 * bT_i ); % q = 1
    kappa = 1. / radius;
    
    if ( radius == Inf(1) )
        radius = 0;
    end
    
    % angle kick
    dalpha = bT_i .* 0.3 ./ norm(P) * lStep;
    alpha = alpha + dalpha;
    
    deltaYRaw = deltaYRaw - DR(1,2);        
end

% Create figures

% Fig 1
figure('name',['BAndKick'],'PaperPositionMode','auto', ...
    'position',[100,0,600,1000]) ;

% Subplot 1
subplot(2,1,1);
hold on;
box on;
grid on;

plot( Rs(:,3), BTotals, '-r');

% xlabel('z (cm)','FontSize',15);
ylabel('B_{total} (T)','FontSize',15);

% line(get(gca,'XLim'),[0,0],'Color','k');

% Subplot 2: Defelction in y
subplot(2,1,2);
hold on;
box on;
grid on;

plot( Rs(:,3), DeltaYRaws, '-b');

% mark cryostat constraints
cryostat_r_inner = 0.00635
cryostat_l = 1.4

line( [ (1-0.5*cryostat_l) (1+0.5*cryostat_l) ], [ -cryostat_r_inner -cryostat_r_inner ] ...
    ,'Color', 'k'...
    ,'LineStyle', ':'...
    );

ylabel('\Delta y [m]','FontSize',15);
xlabel('z [m]','FontSize',15);


% save output
Name = [ BMap sprintf('_%0.3fGeV',pTotal)];

% Save Fig 1
SaveCanvas( Name );

% Save Matlab data file
save( [ 'Output/' Name '.mat' ] , 'theta' , 'phi' , 'pTotal' , 'Rs' , 'DeltaYRaws' );

% Save output txt file
fileID = fopen( ['Output/' Name '.txt' ] , 'w' );
fprintf(fileID,'%6s %6s %6s\n','x','y','z');
fprintf(fileID,'%6.4f %6.4f %6.4f\n', transpose(Rs) );
fclose(fileID);
