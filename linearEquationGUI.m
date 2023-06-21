function linearEquationGUI()
    % Create a figure
    fig = figure('Position', [100, 100, 600, 400]);

    % Create axes for the graph
    axesGraph = axes('Parent', fig, 'Position', [0.1, 0.3, 0.8, 0.6]);

    % Create sliders for coefficients k and b
    sliderK = uicontrol('Style', 'slider', 'Position', [100, 50, 400, 20], 'Min', -10, 'Max', 10, 'Value', 1, 'Callback', @updateGraph);
    sliderB = uicontrol('Style', 'slider', 'Position', [100, 20, 400, 20], 'Min', -10, 'Max', 10, 'Value', 0, 'Callback', @updateGraph);

    % Create labels for sliders
    labelK = uicontrol('Style', 'text', 'Position', [50, 50, 40, 20], 'String', 'k');
    labelB = uicontrol('Style', 'text', 'Position', [50, 20, 40, 20], 'String', 'b');

    % Create text fields for displaying slider values
    textK = uicontrol('Style', 'text', 'Position', [520, 50, 40, 20]);
    textB = uicontrol('Style', 'text', 'Position', [520, 20, 40, 20]);

    % Initialize graph and slider values
    k = get(sliderK, 'Value');
    b = get(sliderB, 'Value');
    x = -10:0.1:10;
    y = k * x + b;
    plot(axesGraph, x, y);

    % Function to update graph and text fields based on slider values
    function updateGraph(~, ~)
        k = get(sliderK, 'Value');
        b = get(sliderB, 'Value');
        y = k * x + b;
        plot(axesGraph, x, y);
        set(textK, 'String', num2str(k));
        set(textB, 'String', num2str(b));
    end
end
