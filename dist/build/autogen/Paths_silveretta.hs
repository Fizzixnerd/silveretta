module Paths_silveretta (
    version,
    getBinDir, getLibDir, getDataDir, getLibexecDir,
    getDataFileName, getSysconfDir
  ) where

import qualified Control.Exception as Exception
import Data.Version (Version(..))
import System.Environment (getEnv)
import Prelude

catchIO :: IO a -> (Exception.IOException -> IO a) -> IO a
catchIO = Exception.catch

version :: Version
version = Version [0,1,0,0] []
bindir, libdir, datadir, libexecdir, sysconfdir :: FilePath

bindir     = "/home/matt/ssd2/@home/matt/src/silveretta/.cabal-sandbox/bin"
libdir     = "/home/matt/ssd2/@home/matt/src/silveretta/.cabal-sandbox/lib/x86_64-linux-ghc-7.10.2/silveretta-0.1.0.0-CMhmAY5hQI0ASzW0sq0Sze"
datadir    = "/home/matt/ssd2/@home/matt/src/silveretta/.cabal-sandbox/share/x86_64-linux-ghc-7.10.2/silveretta-0.1.0.0"
libexecdir = "/home/matt/ssd2/@home/matt/src/silveretta/.cabal-sandbox/libexec"
sysconfdir = "/home/matt/ssd2/@home/matt/src/silveretta/.cabal-sandbox/etc"

getBinDir, getLibDir, getDataDir, getLibexecDir, getSysconfDir :: IO FilePath
getBinDir = catchIO (getEnv "silveretta_bindir") (\_ -> return bindir)
getLibDir = catchIO (getEnv "silveretta_libdir") (\_ -> return libdir)
getDataDir = catchIO (getEnv "silveretta_datadir") (\_ -> return datadir)
getLibexecDir = catchIO (getEnv "silveretta_libexecdir") (\_ -> return libexecdir)
getSysconfDir = catchIO (getEnv "silveretta_sysconfdir") (\_ -> return sysconfdir)

getDataFileName :: FilePath -> IO FilePath
getDataFileName name = do
  dir <- getDataDir
  return (dir ++ "/" ++ name)
