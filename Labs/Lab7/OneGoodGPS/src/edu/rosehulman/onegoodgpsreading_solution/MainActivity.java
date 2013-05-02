package edu.rosehulman.onegoodgpsreading_solution;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.location.Location;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.TextView;
import android.widget.Toast;
import edu.rosehulman.me435;

public class MainActivity extends Activity implements FieldGpsListener, FieldOrientationListener {

	public enum State {
		READY_FOR_MISSION,
		INITIAL_RED_SCRIPT,
		INITIAL_BLUE_SCRIPT,
		WAITING_FOR_GPS,
		DRIVING_HOME,
		WAITING_FOR_PICKUP,
		RUNNING_VOICE_COMMAND,
		SEEKING_HOME
	}
	private State mState = State.READY_FOR_MISSION;
	private long mStateStartTime = 0;
	
	// Various constants and member variable names.
	private static final String TAG = "OneGoodGps";
	private static final double NO_HEADING_KNOWN = 360.0;
	private TextView mCurrentStateTextView, mStateTimeTextView, mGpsInfoTextView, mSensorOrientationTextView;
	private int mGpsCounter = 0;
	private double mCurrentGpsX, mCurrentGpsY, mCurrentGpsHeading;
	private double mCurrentSensorHeading;
	private Handler mCommandHandler = new Handler();
	protected Timer mTimer;
	public static final int LOOP_INTERVAL_MS = 100;
	
	public void loop() {
		mStateTimeTextView.setText("" + getStateTimeMs()/1000);
		
		// Note you have access to the most recent readings here.
//		Log.d(TAG, "GPS (" + mCurrentGpsX + "," + mCurrentGpsY + ") " +
//		   mCurrentGpsHeading + "   Sensor heading " + mCurrentSensorHeading);
		
		switch(mState) {
		case WAITING_FOR_PICKUP:
			if (getStateTimeMs() > 2000) {
				// I did not get picked up.  Seek some more.
				setState(State.SEEKING_HOME);
			}
			break;
		case SEEKING_HOME:
			if (getStateTimeMs() > 6000) {
				// Done moving.  Stop moving to try for pickup.
				setState(State.WAITING_FOR_PICKUP);
			}
			break;
		default:
			// Other states don't need to do anything in the loop. 
			break;
		}
	};
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		mCurrentStateTextView = (TextView) findViewById(R.id.current_state_textview);
		mStateTimeTextView = (TextView) findViewById(R.id.state_time_textview);
		mGpsInfoTextView = (TextView) findViewById(R.id.gps_info_textview);
		mSensorOrientationTextView = (TextView) findViewById(R.id.orientation_textview);
		
		setState(State.READY_FOR_MISSION);
	}

	@Override
	public void onSensorChanged(double fieldHeading, float[] orientationValues) {
		mCurrentSensorHeading = fieldHeading;
		mSensorOrientationTextView.setText(
		    getString(R.string.degrees_format, fieldHeading));
		// Note:
		//  Azimuth is orientationValues[0]
		//    Pitch is orientationValues[1]
		//     Roll is orientationValues[2]
	}

	public void handleRedTeamGo(View view) {
		//Toast.makeText(this, "Go Red Team", Toast.LENGTH_SHORT).show();
		setState(State.INITIAL_RED_SCRIPT);
	}
	public void handleBlueTeamGo(View view) {
		//Toast.makeText(this, "Go Blue Team", Toast.LENGTH_SHORT).show();
		setState(State.INITIAL_BLUE_SCRIPT);
	}

	public void handleFakeGps(View view) {
		//Toast.makeText(this, "Send fake GPS signal", Toast.LENGTH_SHORT).show();
		onLocationChanged(40, 10, 135, null);
	}
	public void handleMissionComplete(View view) {
		//Toast.makeText(this, "Mission Complete", Toast.LENGTH_SHORT).show();
		setState(State.READY_FOR_MISSION);
	}

	@Override
	public void onLocationChanged(double x, double y, double heading,
			Location location) {
		mGpsCounter++;
		mCurrentGpsX = x;
		mCurrentGpsY = y;
		mCurrentGpsHeading = NO_HEADING_KNOWN;
		String gpsInfo = getString(R.string.xy_format, x, y);
		if (heading <= 180.0 && heading > -180.0) {
			gpsInfo += " " + getString(R.string.degrees_format, heading);
			mCurrentGpsHeading = heading;
			if (mState == State.WAITING_FOR_GPS) {
				setState(State.DRIVING_HOME);
			}
		} else {
			gpsInfo += " ---º";
		}
		gpsInfo += "    " + mGpsCounter;
		mGpsInfoTextView.setText(gpsInfo);
	}
	
	@Override
	protected void onStart() {
		super.onStart();

		mTimer = new Timer();
		mTimer.scheduleAtFixedRate(new TimerTask() {
			@Override
			public void run() {
		          runOnUiThread(new Runnable() {
		            public void run() {
		            	loop();
		            }
		          });
			}
		}, 0, LOOP_INTERVAL_MS);
		//mFieldGps.requestLocationUpdates(this);
		//mFieldOrientation.registerListener(this);
	}
	
	@Override
	protected void onStop() {
		super.onStop();
		mTimer.cancel();
		mTimer = null;
		//mFieldGps.removeUpdates();
		//mFieldOrientation.unregisterListener();
	}
	protected long getStateTimeMs() {
		return System.currentTimeMillis() - mStateStartTime;
	}
	
	/**
	 * Setup any initial work for the state.
	 * @param newState State we are transistioning to NOW.
	 */
	private void setState(State newState) {
		mStateStartTime = System.currentTimeMillis();
		switch(newState) {
		case READY_FOR_MISSION:
			mCurrentStateTextView.setText("READY_FOR_MISSION");
			// Do nothing until the button is pressed.
			break;
		case INITIAL_RED_SCRIPT:
			mCurrentStateTextView.setText("INITIAL_RED_SCRIPT");
			mGpsInfoTextView.setText("---");  // Clear GPS display
			runScriptRed01();
			break;
		case INITIAL_BLUE_SCRIPT:
			mCurrentStateTextView.setText("INITIAL_BLUE_SCRIPT");
			mGpsInfoTextView.setText("---");  // Clear GPS display
			runScriptBlue01();
			break;
		case WAITING_FOR_GPS:
			mCurrentStateTextView.setText("WAITING_FOR_GPS");
			// Do nothing until a GPS reading with a heading is received.
			break;
		case DRIVING_HOME:
			mCurrentStateTextView.setText("DRIVING_HOME");
			useArcRadiusToGetHome();
			break;
		case WAITING_FOR_PICKUP:
			mCurrentStateTextView.setText("WAITING_FOR_PICKUP");
			// Remain still for 2 seconds.
			Toast.makeText(this, "Stop", Toast.LENGTH_SHORT).show();
			break;
		case RUNNING_VOICE_COMMAND:
			mCurrentStateTextView.setText("RUNNING_VOICE_COMMAND");
			// Just received a voice command.
			// Can't get here until voice control is implemented.
			Toast.makeText(this, "Just received a voice command (impossible)", Toast.LENGTH_SHORT).show();
			break;
		case SEEKING_HOME:
			mCurrentStateTextView.setText("SEEKING_HOME");
			// Use the GPS and FieldOrientation sensor to find home.
			// Note, you could also drive in a spiral hoping you are close.
			Toast.makeText(this, "Seeking home", Toast.LENGTH_SHORT).show();
			break;	
		}
		mState = newState;
	}

	private void useArcRadiusToGetHome() {
		// TODO
		//   Use the x, y, and heading to determine arc radius info
		//   Use arc radius to set wheel speed and time
		Toast.makeText(this, "Driving arc home", Toast.LENGTH_SHORT).show();
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				//Toast.makeText(MainActivity.this, "Done! (hopefully)", Toast.LENGTH_SHORT).show();
				setState(State.WAITING_FOR_PICKUP);
			}
		}, 6000);
	}

	private void runScriptRed01() {
		Toast.makeText(this, "Red script step 0", Toast.LENGTH_SHORT).show();
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(MainActivity.this, "Red script step 1", Toast.LENGTH_SHORT).show();
			}
		}, 2000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(MainActivity.this, "Red script step 2", Toast.LENGTH_SHORT).show();				
			}
		}, 4000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				setState(State.WAITING_FOR_GPS);
			}
		}, 6000);
	}

	private void runScriptBlue01() {
		Toast.makeText(this, "Blue script step 0", Toast.LENGTH_SHORT).show();
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(MainActivity.this, "Blue script step 1", Toast.LENGTH_SHORT).show();
			}
		}, 2000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(MainActivity.this, "Blue script step 2", Toast.LENGTH_SHORT).show();				
			}
		}, 4000);
		mCommandHandler.postDelayed(new Runnable() {
			@Override
			public void run() {
				setState(State.WAITING_FOR_GPS);
			}
		}, 6000);
		
		
	}

}
