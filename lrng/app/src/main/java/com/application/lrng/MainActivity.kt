package com.application.lrng

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier

import androidx.compose.ui.Alignment
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.application.lrng.ui.theme.LrngTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            LrngTheme {
                    mymain()
                }
            }
        }
    }

@Composable
fun mymain(){
     Column(
         verticalArrangement = Arrangement.Top,
         horizontalAlignment = Alignment.Start,
         modifier = Modifier
             .fillMaxSize()
             .background(Color(0xF8000000))
     ) {
        Text(
            text = "My app",
            fontSize = 32.sp,
            color = Color(0xf8a236ff),
            modifier = Modifier.padding(start = 20.dp, top = 20.dp)
//            font size
//            color
        )
         Text(text = "Eevee",
             color = Color(0xF8B15BFC),
             modifier = Modifier.padding(start = 20.dp)
         )
         Row(Modifier.fillMaxSize(),horizontalArrangement = Arrangement.Center) {
            Text(text = stringResource(id = R.string.eve_desc),
                Modifier.width(200.dp),
                color = Color.White,
                fontSize = 16.sp
            )
            Column {
                Image(painter = painterResource(id = R.drawable.mm), contentDescription = "",Modifier.height(100.dp) )
                Button(
                onClick = { /*TODO*/ },
                shape = RoundedCornerShape(10.dp),
                colors = ButtonDefaults.buttonColors(containerColor = Color(0xF8C280FA)),
                modifier = Modifier.padding(start = 10.dp, top = 5.dp)
                ) {
                    Text(text = "Send")
                    }
            }
//            Image(painter = painterResource(id = R.drawable.mm), contentDescription = "",Modifier.height(100.dp) )
         }
//         Button(
//             onClick = { /*TODO*/ },
//             shape = RoundedCornerShape(10.dp),
//             colors = ButtonDefaults.buttonColors(containerColor = Color.Gray)
//         ) {
//             Text(text = "Send")
//         }
     }
}