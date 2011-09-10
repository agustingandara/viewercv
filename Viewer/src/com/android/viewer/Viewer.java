/*
   Copyright [2011] [Chris McClanahan]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/*
  Used the project MobileEye as a reference for
  getting the camera data and overlaying the camera bitmap.
*/

package com.android.viewer;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;

public class Viewer extends Activity {

	//private static final String TAG = "Main";

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		initActivity();
		executeActivity();
	}

	@Override
	public void onStart() {
		super.onStart();
	}

	@Override
	public void onResume() {
		super.onResume();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onStop() {
		super.onStop();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	}

	private void initActivity() {
		startNextActivity();
	}

	public void executeActivity() {
	}

	private void startNextActivity() {
		Toast t = Toast.makeText(getApplicationContext(), "Starting Camera Activity", Toast.LENGTH_LONG);
		t.show();
		Intent intent = new Intent(getApplicationContext(), CameraActivity.class);
		startActivity(intent);
		finish();
	}

}
