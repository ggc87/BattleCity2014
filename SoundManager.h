/**
 *  SoundManager.h
 *
 *  Original Code : Van Stokes, Jr. (http://www.EvilMasterMindsInc.com) - Aug 05
 *  Modified Code : Steven Gay - mec3@bluewin.ch - Septembre 2005 - Jan 06
 *                  Deniz Sarikaya - daimler3@gmail.com - August 2010
 *                  IO
 *                    Partial Documentation
 *                    =====================
 *
 *  Very simple SoundManager using OpenAl.
 *
 *  For a more complete one, you should see :
 *  OpenAL++ http://alpp.sourceforge.net/
 *
 ****************************************************************************
 *  COMPILE
 *
 *  - Don't forget to link to : ALut.lib and OpenAL32.lib.
 *    The order is important.
 *
 *  - With CodeBlocks there is one warning I didn't resolve :
 *     "Warning: .drectve `/DEFAULTLIB:"uuid.lib" /DEFAULTLIB:"uuid.lib" ' unrecognized"
 *
 ****************************************************************************
 *  USAGE
 *
 * 1. Create the object from the class with createManager()
 *
 * 2. Call the init() function
 *
 * 3. Call the loadDefaultSounds() function to PRE-LOAD audio into the buffers.
 *    This is optional. Review the function to make changes that you need.
 *
 * 4. Set the Listener Location by calling setListenerPosition() function
 *    continually call this as your Listener (camera) position changes!
 *
 * 5. For each object that emits sound, call the loadSound() function.
 *    CAREFUL : The filename must be unique.
 *
 * 6. Optional : For each object you can set the all the parameters of the
 *    sound with setSound() or only the position, velocity and direction with
 *    setSoundPosition().
 *
 * 7. Call the playAudioSource() to play the sound at some event.
 *    This function will play the sound and then stop. It will NOT repeat playing.
 *    Use stopAudioSource() to stop a sound from playing if its still playing
 *
 * 8. Call pauseAudio() or pauseAllAudio() to pause one or all sound(s).
 *    Call resumeAudio() or resumeAllAudio() to resume one or all paused sound(s).
 *
 * 9. When your object is done emitting sounds (when out of range for example)
 *    call releaseAudioSource().
 *    It is important to release your source when you are no longer going to
 *    need it because you are limited in the number of sources you can have.
 *
 * 10. If your objects moves (other than the listener/camera) then
 *    continually update the objects position by calling setSourcePosition().
 *
 ******************************************************************************
 *
 * Additional informations :
 *
 * Ogg Vorbis - http://www.xiph.org/downloads/
 *            - http://www.illiminable.com/ogg/
 * Flac       - http://flac.sourceforge.net/features.html
 * Theora     - http://www.theora.org/
 *
 *
 ******************************************************************************
 *
 * TODO
 *
 * loadOGG()
 * alSourcePause()
 *
 * Use the EAX functions !
 *
 ******************************************************************************/

 #ifndef __SOUNDMANAGER_H__
 #define __SOUNDMANAGER_H__

 #include <stdio.h>
 #include <string>

 // Comment this line if you don't have the EAX 2.0 SDK installed
 //#define _USEEAX

 #ifdef _USEEAX
     #include "eax.h"
 #endif

 // OpenAl version 1.1
 #include <al.h>
 #include <alc.h>
 #include <alut.h>
 #include <vorbis/vorbisfile.h>
 // Modify this as you need.
 #include "OgreVector3.h"
 #include "OgreQuaternion.h"
 #include "Singleton.h"
 #include <entityx.h>
#include "Messages.h"
    //using namespace Ogre;


 // Be very careful with these two parameters
 // It is very dependant on the audio hardware your
 // user is using. It you get too large, it may work
 // on one persons system but not on another.
 // TODO Write a fct testing the hardware !
 #define MAX_AUDIO_BUFFERS   64
 #define MAX_AUDIO_SOURCES   16

 // Used to store sound filenames
 #define MAX_FILENAME_LENGTH 40

 class SoundManager : public Singleton<SoundManager>, public entityx::Receiver<SoundManager>
 {
   private:
        // EAX related
        bool isEAXPresent;
 #ifdef _USEEAX
        // EAX 2.0 GUIDs
        const GUID DSPROPSETID_EAX20_ListenerProperties
            = { 0x306a6a8, 0xb224, 0x11d2, { 0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };

        const GUID DSPROPSETID_EAX20_BufferProperties
            = { 0x306a6a7, 0xb224, 0x11d2, {0x99, 0xe5, 0x0, 0x0, 0xe8, 0xd8, 0xc7, 0x22 } };

        EAXSet eaxSet; // EAXSet function, retrieved if EAX Extension is supported
        EAXGet eaxGet; // EAXGet function, retrieved if EAX Extension is supported
 #endif // _USEEAX

        bool isInitialised;
        ALCdevice* mSoundDevice;
        ALCcontext* mSoundContext;

        std::string mAudioPath;

        bool isSoundOn;

        ALfloat position[3];
        ALfloat velocity[3];
        ALfloat orientation[6];

        // Needed because of hardware limitation
        // Audio sources
        unsigned int mAudioSourcesInUseCount;
        unsigned int mAudioSources[ MAX_AUDIO_SOURCES ];
        bool         mAudioSourceInUse[ MAX_AUDIO_SOURCES ];

        // Audio buffers
        unsigned int mAudioBuffersInUseCount;
        unsigned int mAudioBuffers[ MAX_AUDIO_BUFFERS ];
        bool         mAudioBufferInUse[ MAX_AUDIO_BUFFERS ];
        char         mAudioBufferFileName[ MAX_AUDIO_BUFFERS ][ MAX_FILENAME_LENGTH ];

        // Function to check if the soundFile is already loaded into a buffer
        int locateAudioBuffer( std::string filename );
        int loadAudioInToSystem( const char* filename );
        // TODO bool loadWAV( std::string filename, ALuint pDestAudioBuffer );
        bool loadOGG( std::string filename, ALuint pDestAudioBuffer );
        // TODO bool loadAU( std::string filename, ALuint pDestAudioBuffer );
        std::map<std::string,unsigned int> audioFiles;

    public:

        void receive(const ShootEvent &event);
        void receive(const ObjectDestroyed &);
        void receive(const GameStarted &event);

        void loadFiles();

        static SoundManager* mSoundManager;

        SoundManager( void );
        virtual ~SoundManager( void );
        void selfDestruct( void );
        static SoundManager* createManager( void );
        static SoundManager* getSingletonPtr( void ) { return mSoundManager; };

        bool init( void );
        bool getIsSoundOn( void ) { return isSoundOn; };
        void setAudioPath( char* path ) { mAudioPath = std::string( path ); };

        bool checkALError( void );
        bool checkALError( std::string pMsg );

        /** See http://www.openal.org/windows_enumeration.html for installing other
        *   devices. You should at least have "Generic Hardware".
        */
        std::string listAvailableDevices( void );

        // Aquire an Audio Source
        // filename = pass in the sound file to play for this source (ex. "myfile.wav")
        // audioId   = returns the AudioSource identifier you will need for the PlayAudioSource();
        bool loadAudio( std::string filename, unsigned int *audioId, bool loop );
        bool releaseAudio( unsigned int audioID );

        // Returns true if the audio is started from the beginning
        // false if error or if already playing
        bool playAudio( unsigned int audioId, bool forceRestart );
        bool stopAudio( unsigned int audioID );
        bool stopAllAudio( void );

        bool pauseAudio( unsigned int audioID );
        bool pauseAllAudio( void );
        bool resumeAudio( unsigned int audioID );
        bool resumeAllAudio( void );

        bool setSoundPosition( unsigned int audioID, Ogre::Vector3 position );

        bool setSoundPosition( unsigned int audioID, Ogre::Vector3 position,
            Ogre::Vector3 velocity, Ogre::Vector3 direction );

        bool setSound( unsigned int audioID, Ogre::Vector3 position,
            Ogre::Vector3 velocity, Ogre::Vector3 direction, float maxDistance,
            bool playNow, bool forceRestart, float minGain );

        bool setListenerPosition( Ogre::Vector3 position, Ogre::Vector3 velocity,
            Ogre::Quaternion orientation );
 };

 #endif /*__SOUNDMANAGER_H__*/
