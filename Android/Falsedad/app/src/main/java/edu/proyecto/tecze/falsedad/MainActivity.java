package edu.proyecto.tecze.falsedad;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.Switch;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    CheckBox b1;
    CheckBox b2;
    CheckBox b3;

    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference myRef;// = database.getReference("estado");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        //Estado a = new Estado(false,false,false);
        //myRef.setValue("hola");
        b1  = (CheckBox)findViewById(R.id.b1);
        b2  = (CheckBox)findViewById(R.id.b2);
        b3  = (CheckBox)findViewById(R.id.b3);
        onclick();
        myRef = FirebaseDatabase.getInstance().getReference();
    }


    public void insertarUsEmFireBase(String child,boolean estado){
        myRef.child("estado").child(child).setValue(estado);

    }

    public void onclick(){
        b1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, final boolean isChecked) {
                insertarUsEmFireBase("b1",isChecked);

            }
        });

        b2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                insertarUsEmFireBase("b2",isChecked);
            }
        });

        b3.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                insertarUsEmFireBase("b3",isChecked);
            }
        });
    }




    private class Estado{
        boolean b1;
        boolean b2;
        boolean b3;

        public Estado(){}

        public Estado(boolean b1, boolean b2,boolean b3){
            this.b1 = b1;
            this.b2 = b2;
            this.b3 = b3;
        }
    }
}
