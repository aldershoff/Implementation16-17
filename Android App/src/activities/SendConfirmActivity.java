package activities;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import asynctasks.ConfirmResponseTask;

import com.virt.GCM.R;



/**
 * Activity for showing the pin in more detail
 * 
 * @author KjellZijlemaker
 * 
 */
public class SendConfirmActivity extends Activity {
	private Button btnResponseOK;
	private Button btnResponseCancel;
	private ConfirmResponseTask response;
	private String regID;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_send_confirm);
		
		Intent intent = getIntent();
		regID = intent.getExtras().getString("regID");

		intent.removeExtra("regID");
		
		System.out.println("Gotten regID is: " + regID);

		// Making new response with the desired IP and portnumber
				response = new ConfirmResponseTask("77.163.17.142", 10000, this);
				
				/**
				 * Setting button for registering and listen to it This is for the GCM
				 * server
				 */
				btnResponseOK = (Button) findViewById(R.id.btnConfirm);
				btnResponseOK.setOnClickListener(new View.OnClickListener() {
					@Override
					public void onClick(View arg0) {
						response.new RunConfirmToServer().execute(regID);
					}
				});
				
				/**
				 * Setting button for registering and listen to it This is for the GCM
				 * server
				 */
				btnResponseCancel = (Button) findViewById(R.id.btnCancel);
				btnResponseCancel.setOnClickListener(new View.OnClickListener() {
					@Override
					public void onClick(View arg0) {
						response.new RunConfirmToServer().execute("CANCEL");

					}
				});


	}
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
	    if ((keyCode == KeyEvent.KEYCODE_BACK))
	    {
	        finish();
	    }
	    return super.onKeyDown(keyCode, event);
	}

}
