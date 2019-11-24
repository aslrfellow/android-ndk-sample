package com.example.helloworldcpp1;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;

import android.net.Uri;
import android.os.Bundle;

import android.telephony.SmsMessage;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import java.util.TreeMap;

//https://stackoverflow.com/questions/7089313/android-listen-for-incoming-sms-messages
//https://androidkennel.org/android-camera-access-tutorial/
//https://github.com/kevalpatel2106/android-hidden-camera/blob/master/hiddencamera/src/main/java/com/androidhiddencamera/CameraPreview.java
//https://developer.android.com/guide/topics/media/camera
//https://github.com/botyourbusiness/android-camera2-secret-picture-taker/blob/master/app/src/main/java/com/hzitoun/camera2SecretPictureTaker/services/PictureCapturingServiceImpl.java


public class SmsListener extends BroadcastReceiver implements PictureCapturingListener, ActivityCompat.OnRequestPermissionsResultCallback {

    private SharedPreferences preferences;
    private Uri file;

    //The capture service
    private APictureCapturingService pictureService;


    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[], @NonNull int[] grantResults) {
        System.out.println("SMS onRequestPermissionsResult");
    }


    /**
     * Displaying the pictures taken.
     */
    @Override
    public void onCaptureDone(String pictureUrl, byte[] pictureData) {
        System.out.println("SMS onCaptureDone");
        System.out.println("Picture saved to " + pictureUrl);
    }

    /**
     * We've finished taking pictures from all phone's cameras
     */
    @Override
    public void onDoneCapturingAllPhotos(TreeMap<String, byte[]> picturesTaken) {
        System.out.println("SMS onDoneCapturingAllPhotos");
        if (picturesTaken != null && !picturesTaken.isEmpty()) {
            System.out.println("Done capturing all photos!");
            return;
        }
        System.out.println("No camera detected!");
    }


    @Override
    public void onReceive(Context context, Intent intent) {
        // TODO Auto-generated method stub

        System.out.println("Received!");

//        pictureService = PictureCapturingServiceImpl.getSMSInstance(context);
//        pictureService.startCapturing(this);

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
}