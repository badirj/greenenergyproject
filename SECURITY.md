# Security and Safety Guidelines

## Security Considerations

### API Keys and Credentials

⚠️ **NEVER commit sensitive information to version control!**

Sensitive information includes:
- WiFi passwords
- Firebase API keys and database secrets
- `google-services.json` file
- Any authentication tokens
- Personal information

**What we've done:**
- `.gitignore` excludes `google-services.json`
- Example files provided (`.example` suffix)
- Placeholders in code for credentials

**What you must do:**
1. Copy `google-services.json.example` to `google-services.json`
2. Fill in your actual Firebase credentials
3. Update ESP32 code with your WiFi and Firebase credentials
4. Never share your actual credentials publicly

### Firebase Security Rules

The example rules in [FIRESTORE_SETUP.md](FIRESTORE_SETUP.md) allow all read/write access for development. **This is NOT secure for production!**

**For production, implement proper security rules:**

```javascript
rules_version = '2';
service cloud.firestore {
  match /databases/{database}/documents {
    match /energy_system/{document} {
      // Allow read for authenticated users only
      allow read: if request.auth != null;
      
      // Allow write only from specific sources
      allow write: if request.auth != null && 
                      request.auth.token.email == "your-esp32-service-account@project.iam.gserviceaccount.com";
      
      // Validate data structure
      allow update: if request.resource.data.keys().hasAll(['solar_battery', 'piezo_battery', 'sensors']);
    }
  }
}
```

**Recommended security measures:**
1. Use Firebase Authentication
2. Create service accounts for ESP32
3. Implement data validation rules
4. Set up rate limiting
5. Monitor for unusual access patterns
6. Enable Firebase App Check

## Electrical Safety

### Battery Safety ⚡

**18650 Lithium-ion batteries can be dangerous if mishandled!**

**DO:**
- ✅ Use batteries with built-in protection circuits
- ✅ Charge with proper charger (TP4056 with protection)
- ✅ Monitor temperature during charging
- ✅ Keep away from metal objects
- ✅ Store at 50-60% charge for long-term storage
- ✅ Use in well-ventilated area
- ✅ Have fire extinguisher nearby
- ✅ Dispose properly at battery recycling center

**DON'T:**
- ❌ Never short circuit batteries
- ❌ Never charge unprotected batteries
- ❌ Never exceed 4.2V charging voltage
- ❌ Never discharge below 3.0V
- ❌ Never puncture or disassemble batteries
- ❌ Never expose to heat or fire
- ❌ Never charge damaged batteries
- ❌ Never leave charging unattended initially

**Warning Signs:**
- Battery feels hot (>40°C) during charging → Stop immediately
- Battery swells or bulges → Dispose safely
- Battery smells strange → Disconnect and ventilate area
- Charging takes much longer than normal → Replace battery
- Battery voltage drops rapidly → Battery damaged

### High Voltage Warning ⚡

Piezoelectric disks can generate voltages up to 30V!

**Safety measures:**
- Use proper diodes for rectification
- Ensure supercapacitor rated for voltage
- Never touch piezo contacts directly
- Insulate all connections
- Test with multimeter before connecting to circuits

### General Electrical Safety

**Before powering on:**
1. Visual inspection of all connections
2. Check for shorts with multimeter (continuity test)
3. Verify polarity on all components
4. Test voltage dividers produce correct output
5. Ensure no exposed conductors

**While operating:**
1. Monitor temperature of components
2. Check for unusual smells or sounds
3. Watch for smoke or sparks (disconnect immediately)
4. Keep flammable materials away
5. Have emergency power-off accessible

### Soldering Safety

**Proper soldering practices:**
- Work in well-ventilated area
- Use lead-free solder when possible
- Don't overheat components (especially batteries!)
- Use proper temperature (350-400°C for most work)
- Wear safety glasses
- Keep soldering iron secure when not in use
- Never solder batteries directly - use holders with tabs

### Enclosure Safety

**Proper enclosure design:**
- Adequate ventilation for heat dissipation
- No sharp edges inside
- Secure mounting for all components
- Cable strain relief
- Access for emergency shutdown
- Labels for voltage levels
- Warning labels for battery compartment

## Physical Safety

### Solar Panel Installation

**Mounting considerations:**
- Secure mounting to prevent falling
- Weather-resistant materials
- Consider wind load
- No overhead power lines nearby
- Accessible for cleaning and maintenance

### Servo Motor

**Mechanical safety:**
- Ensure servo has adequate torque
- Check for binding or obstruction
- Secure mounting to prevent stress
- Limit travel to prevent damage
- Consider adding soft stops

## Data Security

### WiFi Security

**Recommendations:**
- Use WPA3 encryption if available (WPA2 minimum)
- Strong WiFi password (16+ characters)
- Separate IoT network (VLAN) if possible
- Disable WPS
- Update router firmware regularly
- Monitor connected devices

### Firebase Security

**Best practices:**
- Enable 2-factor authentication on Google account
- Regular audit of access logs
- Implement IP whitelisting if possible
- Use environment variables for secrets
- Rotate credentials periodically
- Monitor for unauthorized access

### Mobile App Security

**Security measures:**
- Keep app updated
- Review app permissions
- Use device encryption
- Lock device with PIN/biometric
- Don't root/jailbreak device
- Be cautious on public WiFi

## Privacy Considerations

### Data Collection

This system collects:
- Battery voltage and status
- Temperature and humidity readings
- Energy production data
- Servo position
- Timestamps

**Privacy protection:**
- Data stored only in your Firebase project
- No third-party data sharing
- Location data not collected
- No personal information required
- You control data retention

### Data Retention

**Recommendations:**
1. Implement data cleanup for old records
2. Don't store unnecessary personal information
3. Regular backups of important data
4. Secure deletion when decommissioning

## Reporting Security Issues

If you discover a security vulnerability:

1. **DO NOT** open a public issue
2. Email the maintainer privately (if personal project)
3. Describe the vulnerability in detail
4. Allow reasonable time for fix before public disclosure
5. Include steps to reproduce if applicable

## Emergency Procedures

### Battery Fire or Thermal Runaway

1. **If safe to do so:**
   - Disconnect power immediately
   - Use Class D fire extinguisher (for lithium fires)
   - Evacuate area if fire spreads
   - Call emergency services if needed

2. **Never use water on lithium battery fires!**

3. **After incident:**
   - Ventilate area thoroughly
   - Inspect all components
   - Replace damaged parts
   - Document incident for future reference

### Electrical Shock

1. **DO NOT touch person if still in contact with electricity**
2. Disconnect power source if safe
3. Call emergency services
4. Administer first aid if trained
5. Monitor for delayed symptoms

### System Malfunction

1. Press power button to shut down ESP32
2. Disconnect batteries if safe
3. Check for obvious issues (smoke, heat, smell)
4. Document error messages and behavior
5. Don't restart until issue identified

## Testing Safety

**Safe testing procedures:**
1. Test individual components before integration
2. Use current-limited power supply when possible
3. Monitor voltage and current during tests
4. Keep multimeter handy
5. Start with low power/voltage tests
6. Have someone nearby during initial testing
7. Document all tests and results

## Legal Disclaimer

This project is provided "as is" without warranty of any kind. The authors are not responsible for:
- Damage to property
- Personal injury
- Loss of data
- Fires, explosions, or other hazards
- Regulatory compliance in your jurisdiction

**By building this project, you assume all risks.**

## Certifications and Compliance

⚠️ **This is a DIY educational project and is NOT certified for commercial use.**

**Consider these regulations:**
- FCC/CE compliance for radio devices (ESP32)
- UL/IEC safety standards for electrical devices
- Local electrical codes
- Building codes for installations
- Environmental regulations for disposal

**For commercial or public use:**
- Obtain proper certifications
- Professional electrical inspection
- Liability insurance
- Compliance testing
- User safety documentation

## Resources

**Safety Standards:**
- IEC 62368-1 (Audio/video, ICT equipment safety)
- UL 1642 (Lithium batteries)
- IEC 62133 (Secondary cells and batteries)

**Battery Safety:**
- [Battery University - Safety](https://batteryuniversity.com/learn/article/safety_concerns_with_li_ion)
- [NFPA Battery Safety Guidelines](https://www.nfpa.org/)

**Electrical Safety:**
- [OSHA Electrical Safety](https://www.osha.gov/electrical)
- Local electrical safety codes

## Version History

- v1.0 (2025-01) - Initial security and safety guidelines

---

**Remember: Safety first! No project is worth risking injury or property damage.**

**If you're unsure about any safety aspect, consult with a qualified professional before proceeding.**
