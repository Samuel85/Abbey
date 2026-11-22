Module["preRun"].push(function () {
    addRunDependency('syncfs')

    FS.mkdir('/save')
    FS.mount(IDBFS, { autoPersist: true }, '/save')
    FS.syncfs(true, function (err) {
      if (err) throw err
      removeRunDependency('syncfs')
      console.log("FS Synced")
    })
  });

function syncPersist(callbackPtr) {
	FS.syncfs(false, function(err) {
		console.log("syncPersist err "+err);
		if (callbackPtr)
			dynCall('vi', callbackPtr, [err ? 1 : 0]);
	});
}

// Hazla global para que esté realmente accesible por el linker
if (typeof window !== "undefined") window.syncPersist = syncPersist;
if (typeof global !== "undefined") global.syncPersist = syncPersist;
if (typeof self !== "undefined") self.syncPersist = syncPersist;
