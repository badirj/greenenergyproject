package com.badir.greenenergyprojectv2;

import android.graphics.Color;
import android.os.Bundle;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.formatter.ValueFormatter;
import com.google.android.material.switchmaterial.SwitchMaterial;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.ListenerRegistration;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    // Firebase
    private FirebaseFirestore db;
    private DocumentReference energySystemRef;
    private ListenerRegistration firestoreListener;

    // UI Components
    private TextView connectionStatus;
    
    // Solar Battery
    private ProgressBar solarBatteryProgress;
    private TextView solarBatteryPercentage;
    private TextView solarBatteryVoltage;
    private TextView solarBatteryStatus;
    
    // Piezo Battery
    private ProgressBar piezoBatteryProgress;
    private TextView piezoBatteryPercentage;
    private TextView piezoBatteryVoltage;
    private TextView piezoBatteryStatus;
    
    // Sensors
    private TextView temperatureValue;
    private TextView humidityValue;
    
    // Servo Control
    private SeekBar servoSeekBar;
    private TextView servoAngleText;
    private SwitchMaterial protectionModeSwitch;
    
    // Energy Production
    private TextView solarEnergyValue;
    private TextView piezoEnergyValue;
    private TextView efficiencyValue;
    private BarChart energyChart;

    private boolean isUpdatingServo = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize Firebase
        db = FirebaseFirestore.getInstance();
        energySystemRef = db.collection("energy_system").document("main");

        // Initialize UI components
        initializeViews();
        
        // Setup listeners
        setupServoControl();
        setupProtectionMode();
        
        // Setup chart
        setupEnergyChart();
        
        // Start listening to Firestore
        startFirestoreListener();
    }

    private void initializeViews() {
        connectionStatus = findViewById(R.id.connectionStatus);
        
        // Solar Battery
        solarBatteryProgress = findViewById(R.id.solarBatteryProgress);
        solarBatteryPercentage = findViewById(R.id.solarBatteryPercentage);
        solarBatteryVoltage = findViewById(R.id.solarBatteryVoltage);
        solarBatteryStatus = findViewById(R.id.solarBatteryStatus);
        
        // Piezo Battery
        piezoBatteryProgress = findViewById(R.id.piezoBatteryProgress);
        piezoBatteryPercentage = findViewById(R.id.piezoBatteryPercentage);
        piezoBatteryVoltage = findViewById(R.id.piezoBatteryVoltage);
        piezoBatteryStatus = findViewById(R.id.piezoBatteryStatus);
        
        // Sensors
        temperatureValue = findViewById(R.id.temperatureValue);
        humidityValue = findViewById(R.id.humidityValue);
        
        // Servo Control
        servoSeekBar = findViewById(R.id.servoSeekBar);
        servoAngleText = findViewById(R.id.servoAngleText);
        protectionModeSwitch = findViewById(R.id.protectionModeSwitch);
        
        // Energy Production
        solarEnergyValue = findViewById(R.id.solarEnergyValue);
        piezoEnergyValue = findViewById(R.id.piezoEnergyValue);
        efficiencyValue = findViewById(R.id.efficiencyValue);
        energyChart = findViewById(R.id.energyChart);
    }

    private void setupServoControl() {
        servoSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser) {
                    servoAngleText.setText(String.format(Locale.getDefault(), 
                        getString(R.string.servo_angle), progress));
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                isUpdatingServo = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int angle = seekBar.getProgress();
                updateServoAngle(angle);
                isUpdatingServo = false;
            }
        });
    }

    private void setupProtectionMode() {
        protectionModeSwitch.setOnCheckedChangeListener((buttonView, isChecked) -> {
            updateProtectionMode(isChecked);
        });
    }

    private void setupEnergyChart() {
        energyChart.getDescription().setEnabled(false);
        energyChart.setDrawGridBackground(false);
        energyChart.setDrawBarShadow(false);
        energyChart.setHighlightFullBarEnabled(false);
        energyChart.setPinchZoom(false);
        energyChart.setDrawValueAboveBar(true);

        XAxis xAxis = energyChart.getXAxis();
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setDrawGridLines(false);
        xAxis.setGranularity(1f);
        xAxis.setValueFormatter(new ValueFormatter() {
            @Override
            public String getFormattedValue(float value) {
                return value == 0 ? "Solar" : "Piezo";
            }
        });

        energyChart.getAxisLeft().setDrawGridLines(false);
        energyChart.getAxisRight().setEnabled(false);
        energyChart.getLegend().setEnabled(false);
    }

    private void startFirestoreListener() {
        firestoreListener = energySystemRef.addSnapshotListener((snapshot, error) -> {
            if (error != null) {
                connectionStatus.setText(R.string.disconnected);
                connectionStatus.setTextColor(getResources().getColor(android.R.color.holo_red_dark, null));
                Toast.makeText(this, "Error: " + error.getMessage(), Toast.LENGTH_SHORT).show();
                return;
            }

            if (snapshot != null && snapshot.exists()) {
                connectionStatus.setText(R.string.connected);
                connectionStatus.setTextColor(getResources().getColor(android.R.color.holo_green_dark, null));
                updateUI(snapshot);
            } else {
                connectionStatus.setText(R.string.disconnected);
                connectionStatus.setTextColor(getResources().getColor(android.R.color.holo_red_dark, null));
            }
        });
    }

    private void updateUI(@NonNull DocumentSnapshot snapshot) {
        // Update Solar Battery
        Map<String, Object> solarBattery = (Map<String, Object>) snapshot.get("solar_battery");
        if (solarBattery != null) {
            updateBatteryUI(
                solarBattery,
                solarBatteryProgress,
                solarBatteryPercentage,
                solarBatteryVoltage,
                solarBatteryStatus
            );
        }

        // Update Piezo Battery
        Map<String, Object> piezoBattery = (Map<String, Object>) snapshot.get("piezo_battery");
        if (piezoBattery != null) {
            updateBatteryUI(
                piezoBattery,
                piezoBatteryProgress,
                piezoBatteryPercentage,
                piezoBatteryVoltage,
                piezoBatteryStatus
            );
        }

        // Update Sensors
        Map<String, Object> sensors = (Map<String, Object>) snapshot.get("sensors");
        if (sensors != null) {
            Double temp = getDouble(sensors, "temperature");
            Double hum = getDouble(sensors, "humidity");
            
            temperatureValue.setText(String.format(Locale.getDefault(), "%.1f°C", temp));
            humidityValue.setText(String.format(Locale.getDefault(), "%.0f%%", hum));
        }

        // Update Weather Control
        Map<String, Object> weatherControl = (Map<String, Object>) snapshot.get("weather_control");
        if (weatherControl != null && !isUpdatingServo) {
            Long angle = (Long) weatherControl.get("servo_angle");
            Boolean protectionMode = (Boolean) weatherControl.get("protection_mode");
            
            if (angle != null) {
                servoSeekBar.setProgress(angle.intValue());
                servoAngleText.setText(String.format(Locale.getDefault(), 
                    getString(R.string.servo_angle), angle.intValue()));
            }
            
            if (protectionMode != null) {
                protectionModeSwitch.setChecked(protectionMode);
            }
        }

        // Update Daily Stats
        Map<String, Object> dailyStats = (Map<String, Object>) snapshot.get("daily_stats");
        if (dailyStats != null) {
            Double solarEnergy = getDouble(dailyStats, "total_solar_energy");
            Double piezoEnergy = getDouble(dailyStats, "total_piezo_energy");
            Double efficiency = getDouble(dailyStats, "efficiency_ratio");
            
            solarEnergyValue.setText(String.format(Locale.getDefault(), "%.1f Wh", solarEnergy));
            piezoEnergyValue.setText(String.format(Locale.getDefault(), "%.1f Wh", piezoEnergy));
            efficiencyValue.setText(String.format(Locale.getDefault(), "%.0f%%", efficiency));
            
            updateEnergyChart(solarEnergy, piezoEnergy);
        }
    }

    private void updateBatteryUI(Map<String, Object> batteryData,
                                   ProgressBar progressBar,
                                   TextView percentageText,
                                   TextView voltageText,
                                   TextView statusText) {
        Double voltage = getDouble(batteryData, "voltage");
        Long percentage = (Long) batteryData.get("percentage");
        Boolean charging = (Boolean) batteryData.get("charging");

        if (percentage != null) {
            progressBar.setProgress(percentage.intValue());
            percentageText.setText(String.format(Locale.getDefault(), "%d%%", percentage.intValue()));
        }

        if (voltage != null) {
            voltageText.setText(String.format(Locale.getDefault(), "%.2fV", voltage));
        }

        if (charging != null) {
            statusText.setText(charging ? R.string.charging : R.string.not_charging);
            statusText.setTextColor(charging ? 
                getResources().getColor(R.color.battery_charging, null) : 
                getResources().getColor(R.color.text_secondary, null));
        }
    }

    private void updateEnergyChart(double solarEnergy, double piezoEnergy) {
        ArrayList<BarEntry> entries = new ArrayList<>();
        entries.add(new BarEntry(0, (float) solarEnergy));
        entries.add(new BarEntry(1, (float) piezoEnergy));

        BarDataSet dataSet = new BarDataSet(entries, "Energy Production");
        dataSet.setColors(
            getResources().getColor(R.color.solar_color, null),
            getResources().getColor(R.color.piezo_color, null)
        );
        dataSet.setValueTextSize(12f);
        dataSet.setValueTextColor(Color.BLACK);

        BarData barData = new BarData(dataSet);
        energyChart.setData(barData);
        energyChart.invalidate();
    }

    private void updateServoAngle(int angle) {
        Map<String, Object> updates = new HashMap<>();
        updates.put("weather_control.servo_angle", angle);
        
        energySystemRef.update(updates)
            .addOnSuccessListener(aVoid -> 
                Toast.makeText(this, "Servo angle updated to " + angle + "°", Toast.LENGTH_SHORT).show())
            .addOnFailureListener(e -> 
                Toast.makeText(this, "Failed to update servo: " + e.getMessage(), Toast.LENGTH_SHORT).show());
    }

    private void updateProtectionMode(boolean enabled) {
        Map<String, Object> updates = new HashMap<>();
        updates.put("weather_control.protection_mode", enabled);
        
        energySystemRef.update(updates)
            .addOnSuccessListener(aVoid -> 
                Toast.makeText(this, "Protection mode " + (enabled ? "enabled" : "disabled"), 
                    Toast.LENGTH_SHORT).show())
            .addOnFailureListener(e -> 
                Toast.makeText(this, "Failed to update protection mode: " + e.getMessage(), 
                    Toast.LENGTH_SHORT).show());
    }

    private double getDouble(Map<String, Object> map, String key) {
        Object value = map.get(key);
        if (value instanceof Double) {
            return (Double) value;
        } else if (value instanceof Long) {
            return ((Long) value).doubleValue();
        } else if (value instanceof Integer) {
            return ((Integer) value).doubleValue();
        }
        return 0.0;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (firestoreListener != null) {
            firestoreListener.remove();
        }
    }
}