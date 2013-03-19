function varargout = PlateLoaderGUI(varargin)
% PLATELOADERGUI MATLAB code for PlateLoaderGUI.fig
%      PLATELOADERGUI, by itself, creates a new PLATELOADERGUI or raises the existing
%      singleton*.
%
%      H = PLATELOADERGUI returns the handle to a new PLATELOADERGUI or the handle to
%      the existing singleton*.
%
%      PLATELOADERGUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in PLATELOADERGUI.M with the given input arguments.
%
%      PLATELOADERGUI('Property','Value',...) creates a new PLATELOADERGUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before PlateLoaderGUI_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to PlateLoaderGUI_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help PlateLoaderGUI

% Last Modified by GUIDE v2.5 18-Mar-2013 14:13:59

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @PlateLoaderGUI_OpeningFcn, ...
                   'gui_OutputFcn',  @PlateLoaderGUI_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before PlateLoaderGUI is made visible.
function PlateLoaderGUI_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to PlateLoaderGUI (see VARARGIN)

% Choose default command line output for PlateLoaderGUI
handles.output = hObject;
handles.user.to = 1;
handles.user.from = 1;
addImageToAxes('robot_background.jpg', handles.axes1, 0);
handles.user.robot = PlateLoaderSim(get(handles.edit_com, 'String'));
set(handles.text_status, 'String', handles.user.robot.getStatus());
addImageToAxes('gripper_closed_no_plate.jpg', handles.axes2, 100);
handles.user.homePosition2 = get(handles.axes2, 'Position');
addImageToAxes('extended_bars.jpg', handles.axes3, 50);
handles.user.homePosition3 = get(handles.axes3, 'Position');
delete(get(handles.axes3, 'Children'));
set(handles.table_timings, 'Data', handles.user.robot.defaultTimeTable(1:5,2:4));
% Update handles structure
guidata(hObject, handles);

function pos = updateXPosition(hObject, handles)
currentPos = get(handles.axes2, 'Position');
if (currentPos(2) < handles.user.homePosition2(2))
   handles.user.robot.retract();
   updateZPosition(hObject, handles);
end
step = -5;
switch (handles.user.robot.xAxisPosition)
    case 1
        goalPos = handles.user.homePosition2 + [-190 0 0 0];
        for i=currentPos(1):step:(goalPos(1)-step)
            set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [step 0 0 0]);
            pause(0.03);
            guidata(hObject, handles);
        end
        set(handles.axes3, 'Position', handles.user.homePosition3 + [-190 0 0 0]);
    case 2
        goalPos = handles.user.homePosition2 + [-95 0 0 0];
        if (currentPos(1) < goalPos(1))
            step = 5;
        end
        for i=currentPos(1):step:(goalPos(1)-step)
            set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [step 0 0 0]);
            pause(0.03);
            guidata(hObject, handles);
        end
        set(handles.axes3, 'Position', handles.user.homePosition3 + [-95 0 0 0]);
    case 3
        goalPos = handles.user.homePosition2;
        if (currentPos(1) < goalPos(1))
            step = 5;
        end
        for i=currentPos(1):step:(goalPos(1)-step)
            set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [step 0 0 0]);
            pause(0.03);
            guidata(hObject, handles);
        end
        set(handles.axes3, 'Position', handles.user.homePosition3);
    case 4
        goalPos = handles.user.homePosition2 + [95 0 0 0];
        if (currentPos(1) < goalPos(1))
            step = 5;
        end
        for i=currentPos(1):step:(goalPos(1)-step)
            set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [step 0 0 0]);
            pause(0.03);
            guidata(hObject, handles);
        end
        set(handles.axes3, 'Position', handles.user.homePosition3 + [95 0 0 0]);
    case 5
        goalPos = handles.user.homePosition2 + [190 0 0 0];
        if (currentPos(1) < goalPos(1))
            step = 5;
        end
        for i=currentPos(1):step:(goalPos(1)-step)
            set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [step 0 0 0]);
            pause(0.03);
            guidata(hObject, handles);
        end
        set(handles.axes3, 'Position', handles.user.homePosition3 + [190 0 0 0]);
end
guidata(hObject, handles);

function pos = updateZPosition(hObject, handles)
yPos = get(handles.axes2, 'Position');
if (handles.user.robot.isZAxisExtended && yPos(2) == handles.user.homePosition2(2))
    for i=1:24
        set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [0 -5 0 0]);
        pause(0.03);
        guidata(hObject, handles);
    end
    addImageToAxes('extended_bars.jpg', handles.axes3, 50);
elseif (~handles.user.robot.isZAxisExtended && yPos(2) ~= handles.user.homePosition2(2))
    delete(get(handles.axes3, 'Children'));
    for i=1:24
        set(handles.axes2, 'Position', get(handles.axes2, 'Position') + [0 5 0 0]);
        pause(0.03);
        guidata(hObject, handles);
    end
else
    %do nothing
end
guidata(hObject, handles);

function pos = updateGripperPosition(hObject, handles)
if ~handles.user.robot.isGripperClosed
    addImageToAxes('gripper_open_no_plate.jpg', handles.axes2, 100);
elseif handles.user.robot.isPlatePresent
    addImageToAxes('gripper_with_plate.jpg', handles.axes2, 100);
else
    addImageToAxes('gripper_closed_no_plate.jpg', handles.axes2, 100);
end
guidata(hObject, handles);
        


% UIWAIT makes PlateLoaderGUI wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = PlateLoaderGUI_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in button_connect.
function button_connect_Callback(hObject, eventdata, handles)
% hObject    handle to button_connect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.shutdown();
handles.user.robot = PlateLoader(str2double(get(handles.edit_com, 'String')));
guidata(hObject, handles);


% --- Executes on button press in button_disconnect.
function button_disconnect_Callback(hObject, eventdata, handles)
% hObject    handle to button_disconnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.shutdown();
handles.user.robot = PlateLoaderSim(str2double(get(handles.edit_com, 'String')));
guidata(hObject, handles);



function edit_com_Callback(hObject, eventdata, handles)
% hObject    handle to edit_com (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit_com as text
%        str2double(get(hObject,'String')) returns contents of edit_com as a double


% --- Executes during object creation, after setting all properties.
function edit_com_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit_com (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in button_retract.
function button_retract_Callback(hObject, eventdata, handles)
% hObject    handle to button_retract (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.retract();
updateZPosition(hObject, handles);


% --- Executes on button press in button_extend.
function button_extend_Callback(hObject, eventdata, handles)
% hObject    handle to button_extend (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.extend();
updateZPosition(hObject, handles);


% --- Executes on button press in button_open.
function button_open_Callback(hObject, eventdata, handles)
% hObject    handle to button_open (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.open();
updateGripperPosition(hObject, handles);


% --- Executes on button press in button_close.
function button_close_Callback(hObject, eventdata, handles)
% hObject    handle to button_close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.close();
updateGripperPosition(hObject, handles);


% --- Executes on button press in button_set_delay.
function button_set_delay_Callback(hObject, eventdata, handles)
% hObject    handle to button_set_delay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
val = [zeros(5,1), get(handles.table_timings, 'Data'), zeros(5,1)];
handles.user.robot.setTimeValues(val);


% --- Executes on button press in button_reset_delay.
function button_reset_delay_Callback(hObject, eventdata, handles)
% hObject    handle to button_reset_delay (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.table_timings, 'Data', handles.user.robot.defaultTimeTable(1:5,2:4));
guidata(hObject, handles);


% --- Executes on button press in button_reset.
function button_reset_Callback(hObject, eventdata, handles)
% hObject    handle to button_reset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.reset();
updateXPosition(hObject, handles);
updateZPosition(hObject, handles);
updateGripperPosition(hObject, handles);


% --- Executes on button press in button_x_1.
function button_x_1_Callback(hObject, eventdata, handles)
% hObject    handle to button_x_1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(1);
updateXPosition(hObject, handles);


% --- Executes on button press in button_x_2.
function button_x_2_Callback(hObject, eventdata, handles)
% hObject    handle to button_x_2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(2);
updateXPosition(hObject, handles);


% --- Executes on button press in button_x_3.
function button_x_3_Callback(hObject, eventdata, handles)
% hObject    handle to button_x_3 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(3);
updateXPosition(hObject, handles);


% --- Executes on button press in button_x_5.
function button_x_5_Callback(hObject, eventdata, handles)
% hObject    handle to button_x_5 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(5);
updateXPosition(hObject, handles);


% --- Executes on button press in button_x_4.
function button_x_4_Callback(hObject, eventdata, handles)
% hObject    handle to button_x_4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(4);
updateXPosition(hObject, handles);


% --- Executes on selection change in menu_from.
function menu_from_Callback(hObject, eventdata, handles)
% hObject    handle to menu_from (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns menu_from contents as cell array
%        contents{get(hObject,'Value')} returns selected item from menu_from
contents = cellstr(get(hObject, 'String'));
handles.user.from = round(str2double(contents{get(hObject, 'Value')}));
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function menu_from_CreateFcn(hObject, eventdata, handles)
% hObject    handle to menu_from (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in menu_to.
function menu_to_Callback(hObject, eventdata, handles)
% hObject    handle to menu_to (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns menu_to contents as cell array
%        contents{get(hObject,'Value')} returns selected item from menu_to
contents = cellstr(get(hObject, 'String'));
handles.user.to = round(str2double(contents{get(hObject, 'Value')}));
guidata(hObject, handles);


% --- Executes during object creation, after setting all properties.
function menu_to_CreateFcn(hObject, eventdata, handles)
% hObject    handle to menu_to (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in button_move.
function button_move_Callback(hObject, eventdata, handles)
% hObject    handle to button_move (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.from
handles.user.to
handles.user.robot.movePlate(handles.user.from, handles.user.to);
updateXPosition(hObject, handles);


% --- Executes on button press in button_status.
function button_status_Callback(hObject, eventdata, handles)
% hObject    handle to button_status (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
set(handles.text_status, 'String', handles.user.robot.getStatus());
guidata(hObject, handles);


% --- Executes on button press in button_frog.
function button_frog_Callback(hObject, eventdata, handles)
% hObject    handle to button_frog (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(1);
updateXPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(2);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(4);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(5);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.retract();
updateZPosition(hObject, handles);


% --- Executes on button press in button_snake.
function button_snake_Callback(hObject, eventdata, handles)
% hObject    handle to button_snake (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.user.robot.x(2);
updateXPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(1);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(2);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(4);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(2);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(4);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(5);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.x(3);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.close();
updateGripperPosition(hObject, handles);
handles.user.robot.x(4);
updateXPosition(hObject, handles);
handles.user.robot.extend();
updateZPosition(hObject, handles);
handles.user.robot.open();
updateGripperPosition(hObject, handles);
handles.user.robot.retract();
updateZPosition(hObject, handles);
