all: prepare

clean:
	rm -f ./berlin-latest.os*

berlin-latest.osm.pbf:
	wget http://download.geofabrik.de/europe/germany/berlin-latest.osm.pbf

extract: berlin-latest.osm.pbf
	osrm-extract berlin-latest.osm.pbf -p car.lua

prepare: extract
	osrm-prepare berlin-latest.osrm -p car.lua

berlin-latest.osrm: extract
berlin-latest.osrm.names: extract
berlin-latest.osrm.restrictions: extract
berlin-latest.osrm.timestamp: extract
berlin-latest.osrm.edges: prepare
berlin-latest.osrm.fileIndex: prepare
berlin-latest.osrm.geometry: prepare
berlin-latest.osrm.hsgr: prepare
berlin-latest.osrm.nodes: prepare
berlin-latest.osrm.ramIndex: prepare

.PHONY: all clean extract prepare
