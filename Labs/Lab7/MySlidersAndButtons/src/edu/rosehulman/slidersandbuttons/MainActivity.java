package edu.rosehulman.slidersandbuttons;

import java.util.ArrayList;

import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;
import edu.rosehulman.me435.AccessoryActivity;

public class MainActivity extends AccessoryActivity implements OnSeekBarChangeListener {

	private ArrayList<SeekBar> mSeekBars = new ArrayList<SeekBar>();
	private TextView mJointAnglesTextView;
	private TextView mGripperDistanceTextView;
	private Handler mCommandHandler = new Handler(); // Used for scripts
	
	private static final String WHEEL_MODE_REVERSE = "REVERSE";
	private static final String WHEEL_MODE_BRAKE = "BRAKE";
	private static final String WHEEL_MODE_FORWARD = "FORWARD";
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mJointAnglesTextView = (TextView) findViewById(R.id.joint_angles_textview);
		mGripperDistanceTextView = (TextView) findViewById(R.id.gripper_distance_textview);
		mSeekBars.add((SeekBar) findViewById(R.id.gripper_seek_bar)); // Gripper index 0.
		mSeekBars.add((SeekBar) findViewById(R.id.joint_1_seek_bar)); // Joints index 1-5
		mSeekBars.add((SeekBar) findViewById(R.id.joint_2_seek_bar));
		mSeekBars.add((SeekBar) findViewById(R.id.joint_3_seek_bar));
		mSeekBars.add((SeekBar) findViewById(R.id.joint_4_seek_bar));
		mSeekBars.add((SeekBar) findViewById(R.id.joint_5_seek_bar));
		for (SeekBar seekBar : mSeekBars) {
			seekBar.setOnSeekBarChangeListener(this);
		}
	}
	
	@Override
	protected void onCommandReceived(String receivedCommand) {
		super.onCommandReceived(receivedCommand);
		Toast.makeText(this, "Received: " + receivedCommand, Toast.LENGTH_SHORT).show();
		
		if(receivedCommand.equalsIgnoreCase("good_ball")){
			Button Script3 = (Button) findViewById(R.id.script3Button);
			Script3.callOnClick();
		}
		if(receivedCommand.equalsIgnoreCase("bad_ball")){
			Button Script2 = (Button) findViewById(R.id.script2Button);
			Script2.callOnClick();
		}
		if(receivedCommand.equalsIgnoreCase("present_ball")){
			Toast.makeText(this,  "I triggered!", Toast.LENGTH_SHORT).show();
			Button Script1 = (Button) findViewById(R.id.script1Button);
			Script1.callOnClick();
		}
	}
	
	public void updateSlidersForPosition(int joint1Angle, int joint2Angle, int joint3Angle, int joint4Angle, int joint5Angle) {
		mSeekBars.get(1).setProgress(joint1Angle + 90); // Joint 1
		mSeekBars.get(2).setProgress(joint2Angle); // Joint 2
		mSeekBars.get(3).setProgress(joint3Angle + 90); // Joint 3
		mSeekBars.get(4).setProgress(joint4Angle + 180); // Joint 4
		mSeekBars.get(5).setProgress(joint5Angle); // Joint 5
		
		String jointAnglesStr = getString(R.string.joint_angle_format,
				joint1Angle + 90, joint2Angle, joint3Angle + 90,
				joint4Angle + 180, joint5Angle);

		mJointAnglesTextView.setText(jointAnglesStr);
	}
	
	// ------------------------ Button Listeners ------------------------
	public void handleHomeClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		updateSlidersForPosition(0, 90, 0, -90, 90);
		String command = getString(R.string.position_command, 0,90, 0, -90, 90);
		sendCommand(command);
	}
	public void handlePosition1Click(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		updateSlidersForPosition(20, 68, -53, -134, 164);
		String command = getString(R.string.position_command, 20, 68, -53, -134, 164);
		sendCommand(command);
	}
	public void handlePosition2Click(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		updateSlidersForPosition(-9, 0, 26, -2, 164);
		String command = getString(R.string.position_command, -9, 0, 26, -2, 164);
		sendCommand(command);
	}
	public void handleScript1Click(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.position_command, 0,90, 0, -90, 90);
		sendCommand(command);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Ball
			public void run() {
				String command = getString(R.string.position_command, 90, 85, 58, 0, 149);
				sendCommand(command);			
			}		
		}, 2000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Get Ball
			public void run() {
				String command = getString(R.string.position_command, 90, 128, 58, 0, 149);
				sendCommand(command);			
			}		
		}, 4000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Close Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 0);
				sendCommand(command);				
			}		
		}, 6000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Ball
			public void run() {
				String command = getString(R.string.position_command, 90, 85, 58, 0, 149);
				sendCommand(command);			
			}		
		}, 8000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
				sendCommand(command);			
			}		
		}, 10000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 107, 88, 0, 169);
				sendCommand(command);			
			}		
		}, 12000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Open Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 70);
				sendCommand(command);			
			}		
		}, 14000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
				sendCommand(command);			
			}		
		}, 16000);
	}
	public void handleScript2Click(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		updateSlidersForPosition(0, 60, 90, 0, 169);
		String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
		sendCommand(command);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 107, 88, 0, 169);
				sendCommand(command);			
			}		
		}, 2000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Close Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 0);
				sendCommand(command);				
			}		
		}, 4000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
				sendCommand(command);			
			}		
		}, 6000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Ditch Ball
			public void run() {
				String command = getString(R.string.position_command, 90, 180, -2, -77, 149);
				sendCommand(command);			
			}		
		}, 8000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Open Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 70);
				sendCommand(command);				
			}		
		}, 10000);		
	}
	public void handleScript3Click(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		updateSlidersForPosition(0, 60, 90, 0, 169);
		String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
		sendCommand(command);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 107, 88, 0, 169);
				sendCommand(command);			
			}		
		}, 2000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Close Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 0);
				sendCommand(command);			
			}		
		}, 4000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Point 2
			public void run() {
				String command = getString(R.string.position_command, 0, 60, 90, 0, 169);
				sendCommand(command);			
			}		
		}, 6000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Bowl
			public void run() {
				String command = getString(R.string.position_command, 22, 96, -43, -168, 6);
				sendCommand(command);			
			}		
		}, 8000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Bowl
			public void run() {
				String command = getString(R.string.position_command, 22, 53, -143, -162, 6);
				sendCommand(command);					
			}		
		}, 10000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Open Gripper
			public void run() {
				String command = getString(R.string.gripper_command, 70);
				sendCommand(command);			
			}		
		}, 12000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			//Above Bowl
			public void run() {
				String command = getString(R.string.position_command, 22, 96, -43, -168, 6);
				sendCommand(command);			
			}		
		}, 14000);	
	}
	public void handleStopClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.wheel_speed_command, WHEEL_MODE_BRAKE, 0, WHEEL_MODE_BRAKE, 0);
		sendCommand(command);			
	}
	public void handleForwardClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.wheel_speed_command, WHEEL_MODE_FORWARD, 100, WHEEL_MODE_FORWARD, 100);
		sendCommand(command);			
	}
	public void handleBackClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.wheel_speed_command, WHEEL_MODE_REVERSE, 100, WHEEL_MODE_REVERSE, 100);
		sendCommand(command);			
	}
	public void handleLeftClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.wheel_speed_command, WHEEL_MODE_FORWARD, 0, WHEEL_MODE_FORWARD, 200);
		sendCommand(command);			
	}
	public void handleRightClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.wheel_speed_command, WHEEL_MODE_FORWARD, 200, WHEEL_MODE_FORWARD, 0);
		sendCommand(command);			
	}
	public void handleBatteryClick(View view) {
//		Toast.makeText(this, "TODO: Implement button", Toast.LENGTH_SHORT).show();
		String command = getString(R.string.battery_voltage_request);
		sendCommand(command);
		// Warning!  This one is harder.  You need to Toast replies.
		// Do you remember how to receive messages via on onCommandReceived
	}
		
	// ------------------------ OnSeekBarChangeListener ------------------------
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		if (!fromUser) {
			return; // Do nothing if the change is not from the user.
		}
		// For simplicity just do a complete UI refresh of the text views.
		int seekBarValues[] = new int[6];
		seekBarValues[0] = mSeekBars.get(0).getProgress();  // Gripper
		seekBarValues[1] = mSeekBars.get(1).getProgress() - 90; // Joint 1
		seekBarValues[2] = mSeekBars.get(2).getProgress(); // Joint 2
		seekBarValues[3] = mSeekBars.get(3).getProgress() - 90; // Joint 3
		seekBarValues[4] = mSeekBars.get(4).getProgress() - 180; // Joint 4
		seekBarValues[5] = mSeekBars.get(5).getProgress(); // Joint 5
		
		String jointAnglesStr = getString(R.string.joint_angle_format,
				seekBarValues[1], seekBarValues[2], seekBarValues[3],
				seekBarValues[4], seekBarValues[5]);
		String gripperStr = getString(R.string.gripper_format, seekBarValues[0]);

		mJointAnglesTextView.setText(jointAnglesStr);
		mGripperDistanceTextView.setText(gripperStr);
		
		String command = "";
		switch(seekBar.getId()) {
		case R.id.gripper_seek_bar:
			command = getString(R.string.gripper_command, seekBarValues[0]);
			break;
		case R.id.joint_1_seek_bar:
			command = getString(R.string.joint_angle_command, 1, seekBarValues[1]);
			break;
		case R.id.joint_2_seek_bar:
			command = getString(R.string.joint_angle_command, 2, seekBarValues[2]);
			break;
		case R.id.joint_3_seek_bar:
			command = getString(R.string.joint_angle_command, 3, seekBarValues[3]);
			break;
		case R.id.joint_4_seek_bar:
			command = getString(R.string.joint_angle_command, 4, seekBarValues[4]);
			break;
		case R.id.joint_5_seek_bar:
			command = getString(R.string.joint_angle_command, 5, seekBarValues[5]);			
			break;
		}
		// Uncomment this line to send the slider command.
		sendCommand(command);
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {}
	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {}

}
