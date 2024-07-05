import 'dart:ui';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';

class MapPage extends StatefulWidget {
  const MapPage({super.key, required this.title});

  final String title;

  @override
  State<MapPage> createState() => _MapPageState();
}

class _MapPageState extends State<MapPage> {
  GoogleMapController? _controller;
  final Set<Marker> _markers = {};
  late BitmapDescriptor _customIcon;

  @override
  void initState() {
    super.initState();
    _loadCustomMarker();
  }

  Future<void> _loadCustomMarker() async {
    _customIcon = await BitmapDescriptor.asset(
      ImageConfiguration.empty,
      'assets/donde_estas_no_bg.png',
    );
    setState(() {});
  }

  void _onMapCreated(GoogleMapController controller) {
    _controller = controller;

    setState(() {
      _markers.add(
        Marker(
          markerId: const MarkerId('customMarker'),
          position: const LatLng(-23.5505, -46.6333),
          icon: _customIcon,
        ),
      );
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Center(
          child: Text(widget.title),
        ),
      ),
      body: GoogleMap(
        onMapCreated: _onMapCreated,
        initialCameraPosition: const CameraPosition(
          target: LatLng(-8.01755543493397, -34.949981109896285),
          zoom: 17,
        ),
        markers: _markers,
      ),
    );
  }
}
