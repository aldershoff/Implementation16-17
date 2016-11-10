package events;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import org.json.JSONException;
import org.json.JSONObject;

import config.Config;

import android.content.Context;
import android.util.Log;

/**
 * This class is for sharing the ID with the PlainTech server. Will be done with
 * the hep of a POST message to the server
 * 
 * @author KjellZijlemaker
 * 
 */
public class EventRegisterRegID {

	public String shareRegIdWithAppServer(final Context context,
			final String regId, final String username, final String password) {

		/**
		 * Setting the parameters for sending to the server
		 */
		String result = "";
		Map<String, String> paramsMap = new HashMap<String, String>();
		paramsMap.put("regId", regId);
		paramsMap.put("user", username);
		paramsMap.put("password", password);
		paramsMap.put("isAndroid", "1");
		try {
			URL serverUrl = null;
			try {
				serverUrl = new URL(Config.APP_SERVER_URL);
			} catch (MalformedURLException e) {
				Log.e("AppUtil", "URL Connection Error: "
						+ Config.APP_SERVER_URL, e);
				result = "Invalid URL: " + Config.APP_SERVER_URL;
			}

			/**
			 * Making the message
			 */
			StringBuilder postBody = new StringBuilder();
			Iterator<Entry<String, String>> iterator = paramsMap.entrySet()
					.iterator();

			while (iterator.hasNext()) {
				Entry<String, String> param = iterator.next();
				postBody.append(param.getKey()).append('=')
						.append(param.getValue());
				if (iterator.hasNext()) {
					postBody.append('&');
				}
			}
			String body = postBody.toString();
			byte[] bytes = body.getBytes();
			HttpURLConnection httpCon = null;

			/**
			 * Try to send the message to the server
			 */
			try {
				httpCon = (HttpURLConnection) serverUrl.openConnection();
				httpCon.setDoOutput(true);
				httpCon.setUseCaches(false);
				httpCon.setFixedLengthStreamingMode(bytes.length);
				httpCon.setRequestMethod("POST");
				httpCon.setRequestProperty("Content-Type",
						"application/x-www-form-urlencoded;charset=UTF-8");
				OutputStream out = httpCon.getOutputStream();
				out.write(bytes);
				out.close();

				// Getting status from the response server
				int status = httpCon.getResponseCode();

				/**
				 * If response is 200, then the POST was successfully received
				 * and processed If so, check JSON objects
				 */
				if (status == 200) {
					BufferedReader br = new BufferedReader(
							new InputStreamReader(httpCon.getInputStream()));
					StringBuilder sb = new StringBuilder();
					String line;
					while ((line = br.readLine()) != null) {
						sb.append(line + "\n");
					}
					br.close();
					String json = sb.toString();

					JSONObject jsonObj = new JSONObject(json);

					if (jsonObj.has("success")) {
						result = jsonObj.getString("success");
					} else if (jsonObj.has("error")) {
						result = jsonObj.getString("error");
					} else {
						result = "Post Failure." + " Status: " + status;
					}

				}
			} catch (JSONException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} finally {
				if (httpCon != null) {
					httpCon.disconnect();
				}
			}

		} catch (IOException e) {
			result = "Post Failure. Error in sharing with App Server.";
			Log.e("AppUtil", "Error in sharing with App Server: " + e);
		}

		// Returning the result got from the server
		return result;
	}
}
