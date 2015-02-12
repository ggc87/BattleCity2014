/**
 *  SoundManager.cpp
 *
 *  Original Code : Van Stokes, Jr. (http://www.EvilMasterMindsInc.com) - Aug 05
 *  Modified Code : Steven Gay - mec3@bluewin.ch - Septembre 2005
 *                  Deniz Sarikaya - daimler3@gmail.com - August 2010
 *
 */
 #include "SoundManager.h"
#include "MessageManager.h"


using namespace Ogre;

 SoundManager* SoundManager::mSoundManager = NULL;

 /****************************************************************************/
 SoundManager::SoundManager( void )
 {
    mSoundManager = this;

    isInitialised = false;
    isSoundOn = false;
    mSoundDevice = 0;
    mSoundContext = 0;

    mAudioPath = "";

    // EAX related
    isEAXPresent = false;

    // Initial position of the listener
    position[0] = 0.0;
    position[1] = 0.0;
    position[2] = 0.0;

    // Initial velocity of the listener
    velocity[0] = 0.0;
    velocity[1] = 0.0;
    velocity[2] = 0.0;

    // Initial orientation of the listener = direction + direction up
    orientation[0] = 0.0;
    orientation[1] = 0.0;
    orientation[2] = -1.0;
    orientation[3] = 0.0;
    orientation[4] = 1.0;
    orientation[5] = 0.0;

    // Needed because of hardware limitation
    mAudioBuffersInUseCount = 0;
    mAudioSourcesInUseCount = 0;

    for ( int i=0; i < MAX_AUDIO_SOURCES; i++ )
    {
       mAudioSources[i] = 0;
       mAudioSourceInUse[i] = false;
    }

    for ( int i=0; i < MAX_AUDIO_BUFFERS; i++ )
    {
       mAudioBuffers[i] = 0;
       strcpy( mAudioBufferFileName[i], "--" );
       mAudioBufferInUse[i] = false;
    }

   printf("SoudManager Created.\n");
 }

 /****************************************************************************/
 SoundManager::~SoundManager( void )
 {
   // Delete the sources and buffers
   alDeleteSources( MAX_AUDIO_SOURCES, mAudioSources );
   alDeleteBuffers( MAX_AUDIO_BUFFERS, mAudioBuffers );

    // Destroy the sound context and device
    mSoundContext = alcGetCurrentContext();
    mSoundDevice = alcGetContextsDevice( mSoundContext );
    alcMakeContextCurrent( NULL );
    alcDestroyContext( mSoundContext );
    if ( mSoundDevice)
        alcCloseDevice( mSoundDevice );

   printf("SoudManager Destroyed.\n");
 }

 /****************************************************************************/
 void SoundManager::selfDestruct( void )
 {
   if ( getSingletonPtr() ) delete getSingletonPtr();
 }

 /****************************************************************************/
 SoundManager* SoundManager::createManager( void )
 {
   if (mSoundManager == 0)
        mSoundManager = new SoundManager();
    return mSoundManager;
 }

 /****************************************************************************/
 bool SoundManager::init( void )
 {
   // It's an error to initialise twice OpenAl
   if ( isInitialised ) return true;

   // Open an audio device
   mSoundDevice = alcOpenDevice( NULL ); // TODO ((ALubyte*) "DirectSound3D");
   // mSoundDevice = alcOpenDevice( "DirectSound3D" );

   // Check for errors
   if ( !mSoundDevice )
   {
      printf( "SoundManager::init error : No sound device.\n");
      return false;
   }

   mSoundContext = alcCreateContext( mSoundDevice, NULL );
  //   if ( checkAlError() || !mSoundContext ) // TODO seems not to work! why ?
   if ( !mSoundContext )
   {
      printf( "SoundManager::init error : No sound context.\n");
      return false;
   }

   // Make the context current and active
   alcMakeContextCurrent( mSoundContext );
   if ( checkALError( "Init()" ) )
   {
      printf( "SoundManager::init error : could not make sound context current and active.\n");
      return false;
   }

   // Check for EAX 2.0 support and
   // Retrieves function entry addresses to API ARB extensions, in this case,
   // for the EAX extension. See Appendix 1 (Extensions) of
   // http://www.openal.org/openal_webstf/specs/OpenAL1-1Spec_html/al11spec7.html
   //
   // TODO EAX fct not used anywhere in the code ... !!!
   isEAXPresent = alIsExtensionPresent( "EAX2.0" );
   if ( isEAXPresent )
   {
      printf( "EAX 2.0 Extension available\n" );

 #ifdef _USEEAX
        eaxSet = (EAXSet) alGetProcAddress( "EAXSet" );
        if ( eaxSet == NULL )
            isEAXPresent = false;

        eaxGet = (EAXGet) alGetProcAddress( "EAXGet" );
        if ( eaxGet == NULL )
            isEAXPresent = false;

        if ( !isEAXPresent )
            checkALError( "Failed to get the EAX extension functions adresses.\n" );
 #else
        isEAXPresent = false;
        printf( "... but not used.\n" );
 #endif // _USEEAX

   }

   // Create the Audio Buffers
   alGenBuffers( MAX_AUDIO_BUFFERS, mAudioBuffers );
   if (checkALError("init::alGenBuffers:") )
        return false;

   // Generate Sources
   alGenSources( MAX_AUDIO_SOURCES, mAudioSources );
   if (checkALError( "init::alGenSources :") )
        return false;


   // Setup the initial listener parameters
   // -> location
   alListenerfv( AL_POSITION, position );

   // -> velocity
   alListenerfv( AL_VELOCITY, velocity );

   // -> orientation
   alListenerfv( AL_ORIENTATION, orientation );

   // Gain
   alListenerf( AL_GAIN, 1.0 );

   // Initialise Doppler
   alDopplerFactor( 1.0 ); // 1.2 = exaggerate the pitch shift by 20%
   alDopplerVelocity( 343.0f ); // m/s this may need to be scaled at some point

   // Ok
   isInitialised = true;
   isSoundOn = true;

   printf( "SoundManager initialised.\n\n");

   MessageManager::getPtr()->subscribe<ShootEvent>(*this);
   MessageManager::getPtr()->subscribe<ObjectDestroyed>(*this);
   MessageManager::getPtr()->subscribe<GameStarted>(*this);
   return true;

 }

 /****************************************************************************/
 bool SoundManager::checkALError( void )
 {
   ALenum errCode;
   if ( ( errCode = alGetError() ) != AL_NO_ERROR )
   {
      std::string err = "ERROR SoundManager:: ";
      err += (char*) alGetString( errCode );

      printf( "%s\n", err.c_str());
      return true;
   }
   return false;
 }

 /****************************************************************************/
 std::string SoundManager::listAvailableDevices( void )
 {
   std::string str = "Sound Devices available : ";

   if ( alcIsExtensionPresent( NULL, "ALC_ENUMERATION_EXT" ) == AL_TRUE )
   {
        str = "List of Devices : ";
        str += (char*) alcGetString( NULL, ALC_DEVICE_SPECIFIER );
        str += "\n";
   }
   else
        str += " ... eunmeration error.\n";

    return str;
 }

 /****************************************************************************/
 bool SoundManager::checkALError( std::string pMsg )
 {
   ALenum error = 0;

    if ( (error = alGetError()) == AL_NO_ERROR )
    return false;

   char mStr[256];
   switch ( error )
   {
        case AL_INVALID_NAME:
            sprintf(mStr,"ERROR SoundManager::%s Invalid Name", pMsg.c_str());
            break;
        case AL_INVALID_ENUM:
            sprintf(mStr,"ERROR SoundManager::%s Invalid Enum", pMsg.c_str());
            break;
        case AL_INVALID_VALUE:
            sprintf(mStr,"ERROR SoundManager::%s Invalid Value", pMsg.c_str());
            break;
        case AL_INVALID_OPERATION:
            sprintf(mStr,"ERROR SoundManager::%s Invalid Operation", pMsg.c_str());
            break;
        case AL_OUT_OF_MEMORY:
            sprintf(mStr,"ERROR SoundManager::%s Out Of Memory", pMsg.c_str());
            break;
        default:
            sprintf(mStr,"ERROR SoundManager::%s Unknown error (%i) case in testALError()", pMsg.c_str(), error);
            break;
   };

   printf( "%s\n", mStr );

   return true;
 }

 // Attempts to aquire an empty audio source and assign it back to the caller
 // via AudioSourceID. This will lock the source
 /*****************************************************************************/
 bool SoundManager::loadAudio( std::string filename, unsigned int *audioId,
    bool loop )
 {
   if ( filename.empty() || filename.length() > MAX_FILENAME_LENGTH )
        return false;

   if ( mAudioSourcesInUseCount == MAX_AUDIO_SOURCES )
        return false;   // out of Audio Source slots!

   int bufferID = -1;   // Identity of the Sound Buffer to use
   int sourceID = -1;   // Identity of the Source Buffer to use

   alGetError();    // Clear Error Code

   // Check and see if the pSoundFile is already loaded into a buffer
   bufferID = locateAudioBuffer( filename );
   if ( bufferID < 0 )
   {
      // The sound file isn't loaded in a buffer, lets attempt to load it on the fly
      bufferID = loadAudioInToSystem( filename.c_str() );
      if ( bufferID < 0 ) return false;   // failed!
   }

   // If you are here, the sound the requester wants to reference is in a buffer.
   // Now, we need to find a free Audio Source slot in the sound system
   sourceID = 0;

   while ( mAudioSourceInUse[ sourceID ] == true ) sourceID++;

   // When you are here, 'mSourceID' now represents a free Audio Source slot
   // The free slot may not be at the end of the array but in the middle of it.
   *audioId = sourceID;  // return the Audio Source ID to the caller
   mAudioSourceInUse[ sourceID ] = true; // mark this Source slot as in use
   mAudioSourcesInUseCount++;    // bump the 'in use' counter

   // Now inform OpenAL of the sound assignment and attach the audio buffer
   // to the audio source
   alSourcei( mAudioSources[sourceID], AL_BUFFER, mAudioBuffers[bufferID] );

   // Steven : Not in the original code !!!!!
   alSourcei( mAudioSources[sourceID], AL_LOOPING, loop );

   if ( checkALError( "loadSource()::alSourcei" ) )
        return false;

   return true;
 }

 // Function to check and see if the pSoundFile is already loaded into a buffer
 /*****************************************************************************/
 int SoundManager::locateAudioBuffer( std::string filename )
 {
   for ( unsigned int b = 0; b < MAX_AUDIO_BUFFERS; b++ )
   {
      if ( filename == mAudioBufferFileName[b] ) return (int) b; // TODO Careful : converts unsigned to int!
   }
   return -1;      // not found
 }

 // Function to load a sound file into an AudioBuffer
 /*****************************************************************************/
 // Function to load a sound file into an AudioBuffer
  /*****************************************************************************/
  int SoundManager::loadAudioInToSystem( const char* filename )
  {
    //if ( filename.empty() )
      //   return -1;

    // Make sure we have audio buffers available
    if ( mAudioBuffersInUseCount == MAX_AUDIO_BUFFERS ) return -1;

    // Find a free Audio Buffer slot
    int bufferID = 0;      // Identity of the Sound Buffer to use

    while ( mAudioBufferInUse[ bufferID ] == true ) bufferID++;

    // load .wav, .ogg or .au
 /*  if ( filename.find( ".wav", 0 ) != std::string::npos )
     {
        printf(" ---> found Wav\n");
       // When you are here, bufferID now represents a free Audio Buffer slot
         // Attempt to load the SoundFile into the buffer
        if ( !loadWAV( filename, mAudioBuffers[ bufferID ] ) ) return -1;
    }
 */
   /* if ( filename.find( ".ogg", 0 ) != std::string::npos )
    {
        printf(" ---> found ogg\n");
        if ( !loadOGG( filename, mAudioBuffers[bufferID]) ) return -1;
    }*/
    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;
    alutLoadWAVFile((ALbyte*)filename, &format, &data, &size, &freq, &loop);
    alBufferData(mAudioBuffers[ bufferID ] , format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
 /* else if ( filename.find( ".au", 0 ) != std::string::npos )
    {
        printf(" ---> found au\n");
       // TODO if ( !loadAU( filename, mAudioBuffers[mBufferID]) ) return -1;
    }
 */

    // Successful load of the file into the Audio Buffer.
    mAudioBufferInUse[ bufferID ] = true;      // Buffer now in use

    strcpy( mAudioBufferFileName[ bufferID ], filename ); // save the file descriptor

    mAudioBuffersInUseCount++;               // bump the 'in use' counter

    return bufferID;
  }

 /****************************************************************************/
 bool SoundManager::playAudio( unsigned int audioID, bool forceRestart )
 {
   // Make sure the audio source ident is valid and usable
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[audioID])
        return false;

   int sourceAudioState = 0;

   alGetError();

   // Are we currently playing the audio source?
   alGetSourcei( mAudioSources[audioID], AL_SOURCE_STATE, &sourceAudioState );

   if ( sourceAudioState == AL_PLAYING )
   {
      if ( forceRestart )
           stopAudio( audioID );
        else
            return false; // Not forced, so we don't do anything
   }

   alSourcePlay( mAudioSources[ audioID ] );
   if ( checkALError( "playAudio::alSourcePlay: ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::pauseAudio( unsigned int audioID )
 {
   // Make sure the audio source ident is valid and usable
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[audioID] )
        return false;

   alGetError();

   alSourcePause( mAudioSources[audioID] );

   if ( checkALError( "pauseAudio::alSourceStop ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::pauseAllAudio( void )
 {
   if ( mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES )
        return false;

   alGetError();

   alSourcePausev( mAudioSourcesInUseCount, mAudioSources );

   if ( checkALError( "pauseAllAudio::alSourceStop ") )
        return false;

    return true;
 }

 // We could use playAudio instead !
 /****************************************************************************/
 bool SoundManager::resumeAudio( unsigned int audioID )
 {
   // Make sure the audio source ident is valid and usable
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[audioID] )
        return false;

   alGetError();

   // If the sound was paused the sound will resume, else it will play from
   // the beginning !
   // TODO No check for forced restart. Verify if it is what you want ?
   alSourcePlay( mAudioSources[ audioID ] );

   if ( checkALError( "resumeAudio::alSourceStop ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::resumeAllAudio( void )
 {
   if ( mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES )
        return false;

   alGetError();

   int sourceAudioState = 0;

   for ( int i=0; i<mAudioSourcesInUseCount; i++ )
   {
       // Are we currently playing the audio source?
       alGetSourcei( mAudioSources[i], AL_SOURCE_STATE, &sourceAudioState );

       if ( sourceAudioState == AL_PAUSED )
       {
           resumeAudio( i );
       }
   }

   if ( checkALError( "resumeAllAudio::alSourceStop ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::stopAudio( unsigned int audioID )
 {
   // Make sure the audio source ident is valid and usable
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[audioID] )
        return false;

   alGetError();

   alSourceStop( mAudioSources[audioID] );

   if ( checkALError( "stopAudio::alSourceStop ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::stopAllAudio( void )
 {
   if ( mAudioSourcesInUseCount >= MAX_AUDIO_SOURCES )
        return false;

   alGetError();

   for ( int i=0; i<mAudioSourcesInUseCount; i++ )
   {
       stopAudio( i );
   }

   if ( checkALError( "stopAllAudio::alSourceStop ") )
        return false;

    return true;
 }

 /****************************************************************************/
 bool SoundManager::releaseAudio( unsigned int audioID )
 {
   if ( audioID >= MAX_AUDIO_SOURCES )
        return false;
   alSourceStop( mAudioSources[audioID] );
   mAudioSourceInUse[ audioID ] = false;
   mAudioSourcesInUseCount--;
    return true;
 }

 /****************************************************************************/
 bool SoundManager::setSound( unsigned int audioID, Vector3 position,
    Vector3 velocity, Vector3 direction, float maxDistance,
    bool playNow, bool forceRestart, float minGain )
 {
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[ audioID ] )
        return false;

   // Set the position
   ALfloat pos[] = { position.x, position.y, position.z };

   alSourcefv( mAudioSources[ audioID ], AL_POSITION, pos );

   if ( checkALError( "setSound::alSourcefv:AL_POSITION" ) )
       return false;

   // Set the veclocity
   ALfloat vel[] = { velocity.x, velocity.y, velocity.z };

   alSourcefv( mAudioSources[ audioID ], AL_VELOCITY, vel );

   if ( checkALError( "setSound::alSourcefv:AL_VELOCITY" ) )
       return false;

   // Set the direction
   ALfloat dir[] = { velocity.x, velocity.y, velocity.z };

   alSourcefv( mAudioSources[ audioID ], AL_DIRECTION, dir );

   if ( checkALError( "setSound::alSourcefv:AL_DIRECTION" ) )
       return false;

   // Set the max audible distance
   alSourcef( mAudioSources[ audioID ], AL_MAX_DISTANCE, maxDistance );

   // Set the MIN_GAIN ( IMPORTANT - if not set, nothing audible! )
   alSourcef( mAudioSources[ audioID ], AL_MIN_GAIN, minGain );

   // Set the max gain
   alSourcef( mAudioSources[ audioID ], AL_MAX_GAIN, 1.0f ); // TODO as parameter ? global ?

   // Set the rollof factor
   alSourcef( mAudioSources[ audioID ], AL_ROLLOFF_FACTOR, 1.0f ); // TODO as parameter ?

   // Do we play the sound now ?
   if ( playNow ) return playAudio( audioID, forceRestart ); // TODO bof... not in this fct

   return true;
 }

 /****************************************************************************/
 bool SoundManager::setSoundPosition( unsigned int audioID, Vector3 position )
 {
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[ audioID ] )
        return false;

   // Set the position
   ALfloat pos[] = { position.x, position.y, position.z };

   alSourcefv( mAudioSources[ audioID ], AL_POSITION, pos );

   if ( checkALError( "setSound::alSourcefv:AL_POSITION" ) )
       return false;

   return true;
 }

 /****************************************************************************/
 bool SoundManager::setSoundPosition( unsigned int audioID, Vector3 position,
    Vector3 velocity, Vector3 direction )
 {
   if ( audioID >= MAX_AUDIO_SOURCES || !mAudioSourceInUse[ audioID ] )
        return false;

   // Set the position
   ALfloat pos[] = { position.x, position.y, position.z };

   alSourcefv( mAudioSources[ audioID ], AL_POSITION, pos );

   if ( checkALError( "setSound::alSourcefv:AL_POSITION" ) )
       return false;

   // Set the veclocity
   ALfloat vel[] = { velocity.x, velocity.y, velocity.z };

   alSourcefv( mAudioSources[ audioID ], AL_VELOCITY, vel );

   if ( checkALError( "setSound::alSourcefv:AL_VELOCITY" ) )
       return false;

   // Set the direction
   ALfloat dir[] = { velocity.x, velocity.y, velocity.z };

   alSourcefv( mAudioSources[ audioID ], AL_DIRECTION, dir );

   if ( checkALError( "setSound::alSourcefv:AL_DIRECTION" ) )
       return false;

   return true;
 }

 /****************************************************************************/
 bool SoundManager::setListenerPosition( Vector3 position, Vector3 velocity,
    Quaternion orientation )
 {
   Vector3 axis;

   // Set the position
   ALfloat pos[] = { position.x, position.y, position.z };

   alListenerfv( AL_POSITION, pos );

   if ( checkALError( "setListenerPosition::alListenerfv:AL_POSITION" ) )
       return false;

   // Set the veclocity
   ALfloat vel[] = { velocity.x, velocity.y, velocity.z };

   alListenerfv( AL_VELOCITY, vel );

   if ( checkALError( "setListenerPosition::alListenerfv:AL_VELOCITY" ) )
       return false;

   // Orientation of the listener : look at then look up
   axis = Vector3::ZERO;
   axis.x = orientation.getYaw().valueRadians();
   axis.y = orientation.getPitch().valueRadians();
   axis.z = orientation.getRoll().valueRadians();

   // Set the direction
   ALfloat dir[] = { axis.x, axis.y, axis.z };

   alListenerfv( AL_ORIENTATION, dir );

   if ( checkALError( "setListenerPosition::alListenerfv:AL_DIRECTION" ) )
       return false;

   // TODO as parameters ?
   alListenerf( AL_MAX_DISTANCE, 10000.0f );
   alListenerf( AL_MIN_GAIN, 0.0f );
   alListenerf( AL_MAX_GAIN, 1.0f );
   alListenerf( AL_GAIN, 1.0f );

   return true;
 }

 bool SoundManager::loadOGG( std::string filename, ALuint pDestAudioBuffer )
 {
    OggVorbis_File oggfile;

    if(ov_fopen(const_cast<char*>(filename.c_str()), &oggfile))
    {
        printf("SoundManager::loadOGG() : ov_fopen failed.\n");
        return false;
    }

    vorbis_info* info = ov_info(&oggfile, -1);

    ALenum format;
    switch(info->channels)
    {
        case 1:
            format = AL_FORMAT_MONO16; break;
        case 2:
            format = AL_FORMAT_STEREO16; break;
        case 4:
            format = alGetEnumValue("AL_FORMAT_QUAD16"); break;
        case 6:
            format = alGetEnumValue("AL_FORMAT_51CHN16"); break;
        case 7:
            format = alGetEnumValue("AL_FORMAT_61CHN16"); break;
        case 8:
            format = alGetEnumValue("AL_FORMAT_71CHN16"); break;
        default:
            format = 0; break;
    }

    std::vector<int16> samples;
    char tmpbuf[4096];
    int section = 0;
    bool firstrun = true;
    while(1)
    {
        int result = ov_read(&oggfile, tmpbuf, 4096, 0, 2, 1, &section);
        if(result > 0)
        {
            firstrun = false;
            samples.insert(samples.end(), tmpbuf, tmpbuf + (result));
        }
        else
        {
            if(result < 0)
            {
                printf("SoundManager::loadOGG() : Loading ogg sound data failed!");
                ov_clear(&oggfile);
                return false;
            }
            else
            {
                if(firstrun)
                    return false;
                break;
            }
        }
    }

    alBufferData(pDestAudioBuffer, format, &samples[0], ov_pcm_total(&oggfile, -1), info->rate);

    return true;
 }


void SoundManager::loadFiles(){
    std::cout<<"carico i files audio"<<std::endl;
    this->setAudioPath((char*)".");
    unsigned audioid;
    this->loadAudio("./Media/Audio/explode.wav",&audioid,false);
    audioFiles["explosion"]=audioid;
    this->loadAudio("./Media/Audio/laser7.wav",&audioid,false);
    audioFiles["laser"]=audioid;
    this->loadAudio("./Media/Audio/Intro.wav", &audioid, false);
    audioFiles["intro"] = audioid;
}

void SoundManager::receive(const ShootEvent &event){
    this->playAudio(audioFiles["laser"],false);
}

void SoundManager::receive(const ObjectDestroyed &event){
    this->playAudio(audioFiles["explosion"],false);
}

void SoundManager::receive(const GameStarted &event){
    std::cout << "ricevuto gamestarted" <<std::endl;
    this->playAudio(audioFiles["intro"], false);
}
