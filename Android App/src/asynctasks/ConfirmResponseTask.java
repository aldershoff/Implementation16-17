package asynctasks;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.UnknownHostException;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;

/**
 * Error codes // Connection with server
 * 
 * 101: Unknown Host 
 * 102: Problem with the IO when connecting to server 
 * 103:Problem with setting the timeout for the socket
 * 
 * 
 * Error codes // Inputstream with server
 * 
 * 201: Problem with the IO from getting inputstream from socket 
 * 202: Unsupported coding when getting response back from server (UTF error) 
 * 203: Problem with the IO when reading the inputstream from socket
 * 0: Did not get valid response
 * 
 * 
 * 
 * This class is for making a connection to the server and making a response for
 * the message that was send to this mobile.
 * If the message is the same as on the server, the authentication was OK
 * else there is an error
 * 
 * 
 * @author KjellZijlemaker
 * 
 */
public class ConfirmResponseTask {

	private String dstAddress;
	private int dstPort;
	private ProgressDialog dialog;
	private Context context;


	/**
	 * Constructor when not having an intent
	 * @param addr
	 * @param port
	 * @param context
	 */
	public ConfirmResponseTask(String addr, int port, Context context) {
		this.dstAddress = addr;
		this.dstPort = port;
		this.context = context;
	}

	/**
	 * AsyncTask for making a request to the server socket
	 * @author KjellZijlemaker
	 *
	 */
	public class RunConfirmToServer extends AsyncTask<String, Void, Integer> {
		@Override
		protected void onPreExecute() {
			dialog = new ProgressDialog(context);
			dialog.setMessage("Loading...");
			dialog.show();
		}

		@Override
		protected Integer doInBackground(String... arg0) {
			java.net.Socket socket = null;
			
			/**
			 * Try to connect to the address and port
			 */
			try {
				socket = new java.net.Socket(dstAddress, dstPort);
			} catch (UnknownHostException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
				return 101;
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
				return 102;
			}

			try {
				OutputStreamWriter out = null;
				
				/**
				 * Try to make output for writing and inputstream for reading
				 */
				try {
					out = new OutputStreamWriter(socket.getOutputStream());
				} catch (IOException e2) {
					// TODO Auto-generated catch block
					e2.printStackTrace();
				}
				InputStreamReader in = null;
				try {
					in = new InputStreamReader(socket.getInputStream());
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}


				// Get the message
				String msg = arg0[0];

				System.out.println("message to send is: " + msg);

				// send the message
				PrintWriter pw = new PrintWriter(out, true);
				pw.print(msg);
				pw.flush();

				
				// receive the reply
				BufferedReader br = new BufferedReader(in);
				char[] buffer = new char[300];
				int count = 0;
				try {
					count = br.read(buffer, 0, 300);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
				// Print the reply
				String reply = new String(buffer, 0, count);
				System.out.println(reply);

				// Check the reply
				switch (reply) {
				case "OK":
					return 1;
				case "CANCELOK":
					return 2;
				case "ERROR":
					return 3;
				}

			} finally {
				try {
					socket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			return 1;
		}

		@Override
		protected void onPostExecute(Integer result) {
			// textResponse.setText(response);
			super.onPostExecute(result);
			System.out.println("the result is: " + result);
			checkResponse(result);
			dialog.dismiss();

		}
	}

	/**
	 * Check what response there shall be given
	 * @param serverResponse
	 */
	private void checkResponse(int serverResponse) {
		String message = "";

		try {
			switch (serverResponse) {
			case 1:
				message = ("Confirming was success");
				break;
			case 2:
				message = ("Canceling was success");
				break;
			case 3:
				message = ("Confirming failed...");
				break;
			case 101:
				message = ("Unknown host");
				break;
			case 102:
				message = ("Problem with the IO when connecting to server");
				break;
			case 103:
				message = ("Problem with setting the timeout for the socket");
				break;
			case 201:
				message = ("Problem with the IO from getting inputstream from socket ");
				break;
			case 202:
				message = ("Unsupported coding when getting response back from server (UTF error)");
				break;
			case 203:
				message = ("Problem with the IO when reading the inputstream");
				break;
			default:
				message = ("Did not get any message back...");
			}
		} catch (Exception e) {
			message = ("Very big error");
		} finally {
			makeAlertDialog(message);
		}

	}

	/**
	 * Make the alertDialog for informing the user
	 * @param message
	 */
	private void makeAlertDialog(String message) {
		AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(
				context);

		// set title
		alertDialogBuilder.setTitle("Response from server");

		// set dialog message
		alertDialogBuilder.setMessage(message).setCancelable(false)
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
					
						// Close the activity
						((Activity) context).finish();
					}
				});

		// create alert dialog
		AlertDialog alertDialog = alertDialogBuilder.create();

		// show it
		alertDialog.show();
	}

}