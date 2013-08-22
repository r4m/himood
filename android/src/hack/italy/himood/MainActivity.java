package hack.italy.himood;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URI;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

	private String MAIN_URI = "/mood.cgi?status=";
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
		
		Button sleeper = (Button) findViewById(R.id.sleeper);
		sleeper.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	try {
            		EditText et = (EditText) findViewById(R.id.openpicus);
            		String host = et.getText().toString();
					executeHttpGet("http://" + host + MAIN_URI + 0);
				} catch (Exception e) {
					e.printStackTrace();
				}
            } });
		
		Button slacker = (Button) findViewById(R.id.slacker);
		slacker.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	try {
            		EditText et = (EditText) findViewById(R.id.openpicus);
            		String host = et.getText().toString();
					executeHttpGet("http://" + host + MAIN_URI + 1);
				} catch (Exception e) {
					e.printStackTrace();
				}
            } });
		
		Button rocker = (Button) findViewById(R.id.rocker);
		rocker.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	try {            		
            		EditText et = (EditText) findViewById(R.id.openpicus);
        		String host = et.getText().toString();
				executeHttpGet("http://" + host + MAIN_URI +  2);
				} catch (Exception e) {
					e.printStackTrace();
				}
            } });
		
		Button hacker = (Button) findViewById(R.id.hacker);
		hacker.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	try {            		
            		EditText et = (EditText) findViewById(R.id.openpicus);
        		String host = et.getText().toString();
				executeHttpGet("http://" + host + MAIN_URI + + 3);
				} catch (Exception e) {
					e.printStackTrace();
				}
            } });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    public void executeHttpGet(String uri) throws Exception
    {
        BufferedReader in = null;
        try {
            HttpClient client = new DefaultHttpClient();
            HttpGet request = new HttpGet();
            request.setURI(new URI(uri));
            HttpResponse response = client.execute(request);
            in = new BufferedReader
            (new InputStreamReader(response.getEntity().getContent()));
            StringBuffer sb = new StringBuffer("");
            String line = "";
            String NL = System.getProperty("line.separator");
            while ((line = in.readLine()) != null) {
                sb.append(line + NL);
            }
            in.close();
            String page = sb.toString();
            System.out.println(page);
            } finally {
            if (in != null) {
                try {
                    in.close();
                    } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    
}
