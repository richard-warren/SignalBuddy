%% MAKE SIGNALBUDDY CARTOON

% This is a ridiculous bit of code that makes a silly GIF of SignalBuddy
% with signals shooting out of his head. What an incredible waste of time.

%% SETTINGS

% animation
duration = 2; % seconds
saveVideo = true;

% gobal
lineColor = [.2 .2 .2];
lineWidth = [2 4];  % start and end line widths
xWidth = 12;

% signals
periods = 20;
sigHgt = .3;

% face
faceWid = 1.9;
faceHgt = 2.7;
% faceColor = [[3 255 251] / 255; 1 .5 .5];  % start and stop color
faceColor = [[3 255 251]*.7/255; [3 255 251]/255];  % start and stop color

% eyes
eyeX = [-.75 .4]+.05;
eyeY = [.4 1];
eyeThickness = .1;
eyeColor = [255 255 0] / 255;
squinting = 0;  % by what portion should eyes close during animation

% pupils
pupRad = [.05 .2];  % start and stop radius

% mouth
mouthX = eyeX;
mouthY = -.05;
mouthHgt = .2;  % start and end height
mouthOpening = [.1 .2];  % start and end height
mouseColor = [221 100 255] / 255;

% ears
earWid = .25;
earHgt = .75;
earColor = [255 50 0] / 255;



%% DRAW SIGNALBUDDY

% make figure
close all
figure('color', 'white', 'position', [200 200 1200 400], 'menubar', 'none'); hold on

% sin
sigX = linspace(-xWidth/2, xWidth/2, 1000);
sinY = sin(linspace(0, 2*pi*periods, length(sigX)))*sigHgt/2 + sigHgt*1.5;
sinPlot = plot(sigX, sinY, 'LineWidth', lineWidth(1));

% step
stepY = zeros(1,length(sigX));
rises = round(1:length(sigX)/periods*4:length(sigX));
stepY(rises) = 1;
stepY(rises+floor(length(sigX)/periods/2*4)) = -1;
stepY = (cumsum(stepY)-.5)*sigHgt;
stepPlot = plot(sigX, stepY, 'LineWidth', lineWidth(1));

% pulse
pulseY = zeros(1,length(sigX));
rises = round(1:length(sigX)/periods:length(sigX));
pulseY(rises) = 1;
pulseY(rises+floor(length(sigX)/periods/2)) = -1;
pulseY = (cumsum(pulseY)-.5)*sigHgt - sigHgt*1.5;
pulsePlot = plot(sigX, pulseY, 'LineWidth', lineWidth(1));



% ears
rectangle('position', [-faceWid/2-earWid -earHgt/2 faceWid+2*earWid earHgt], ...
    'LineWidth', lineWidth(1), 'Curvature', .1, 'faceColor', earColor, 'EdgeColor', lineColor);
% face
face = rectangle('position', [-faceWid/2 -faceHgt/2 faceWid faceHgt], ...
                 'LineWidth', lineWidth(1), 'Curvature', .1, 'faceColor', faceColor(1,:), 'EdgeColor', lineColor);
% eyes and pupils
eyeWid = diff(eyeX)/4;  % eye width
theta = linspace(3*pi/2, pi, 20);
px = cos(theta)*pupRad(1);
py = sin(theta)*pupRad(1);
eyes = patch([eyeX(1) eyeX(2) px+eyeX(2) eyeX(2)-eyeWid eyeX(2)-eyeWid eyeX(2)-2*eyeWid px+eyeX(1)+2*eyeWid eyeX(2)-3*eyeWid eyeX(2)-3*eyeWid eyeX(1)], ...
             [eyeY(1) eyeY(1) py+eyeY(2) eyeY(2) eyeY(1)+eyeThickness eyeY(1)+eyeThickness py+eyeY(2) eyeY(2) eyeY(1)+eyeThickness eyeY(1)+eyeThickness], ...
             eyeColor, 'edgecolor', lineColor, 'linewidth', lineWidth(1));
% mouth
x = linspace(mouthX(1), mouthX(2), 100);
y = sin(linspace(0,4*pi,length(x))) * mouthHgt / 2 + mouthY;
mouth = patch([x fliplr(x)], ...
              [y+mouthOpening(1)/2 fliplr(y)-mouthOpening(1)], ...
              mouseColor, 'edgecolor', lineColor, 'linewidth', lineWidth(1));
set(gca, 'xlim', [-1 1]*xWidth/2, 'Visible', 'off', 'DataAspectRatio', [1 1 1])


%% ANIMATE

if saveVideo
    vidWriter = VideoWriter('SignalBuddy.mp4', 'MPEG-4');
    set(vidWriter, 'FrameRate', 30);
    open(vidWriter);
    steps = duration * 30;
else
    steps = 100;
end

faceColors = interp2(1:3, [0 1]', faceColor, 1:3, linspace(0,1,steps)');
mouthOpenings = linspace(mouthOpening(1), mouthOpening(2), steps);
pupRads = linspace(pupRad(1), pupRad(2), steps);
squints = linspace(0, squinting, steps);
lineWidths = linspace(lineWidth(1), lineWidth(2), steps);


for i = [1:steps fliplr(1:steps-1)]
    
    % face color
    set(face, 'faceColor', faceColors(i,:))
    
    % open mouth
    y = sin(linspace(0,4*pi,length(x))) * mouthHgt / 2 + mouthY;
    set(mouth, 'YData', [y+mouthOpenings(i)/2 fliplr(y)-mouthOpenings(i)])
    
    % open eyes
    px = cos(theta)*pupRads(i);
    py = sin(theta)*pupRads(i);
    dy = diff(eyeY)*squints(i);
    set(eyes, 'XData', [eyeX(1) eyeX(2) px+eyeX(2) eyeX(2)-eyeWid eyeX(2)-eyeWid eyeX(2)-2*eyeWid px+eyeX(1)+2*eyeWid eyeX(2)-3*eyeWid eyeX(2)-3*eyeWid eyeX(1)], ...
              'YData', [eyeY(1) eyeY(1) py+eyeY(2)-dy eyeY(2)-dy eyeY(1)+eyeThickness eyeY(1)+eyeThickness py+eyeY(2)-dy eyeY(2)-dy eyeY(1)+eyeThickness eyeY(1)+eyeThickness]); 
    
    % get inds to show for signals
    smpNum = round(length(sigX)*(i/steps));  % number of samples to show
    inds = round([-smpNum:smpNum]/2) + floor(length(sigX)/2);     % inds of samples
    inds = inds(inds>0); inds = inds(inds<=length(sigX));
    
    % update signals
    set(sinPlot, 'XData', sigX(inds), 'YData', sinY(inds), 'LineWidth', lineWidths(i))
    set(stepPlot, 'XData', sigX(inds), 'YData', stepY(inds), 'LineWidth', lineWidths(i))
    set(pulsePlot, 'XData', sigX(inds), 'YData', pulseY(inds), 'LineWidth', lineWidths(i))
    
    % get frame and write to video
    if saveVideo
        frame = getframe(gcf);
        writeVideo(vidWriter, frame);
        pause(.001)
    else
        pause(.01)
    end
end

if saveVideo; close(vidWriter); end

