import 'package:donde_estas/pages/map_page.dart';
import 'package:flutter/material.dart';
import 'package:donde_estas/firebase_options.dart';
import 'package:firebase_core/firebase_core.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MapPage(title: '¿donde estás?'),
    );
  }
}

// class DondeEstasMap extends StatelessWidget {
//   const DondeEstasMap({super.key});

//   @override
//   Widget build(BuildContext context) {
//     return FlutterMap(
//       options: const MapOptions(
//         initialCenter: LatLng(-8.01755543493397, -34.949981109896285),
//         initialZoom: 17,
//         // interactionOptions: InteractionOptions(),
//       ),
//       children: [
//         TileLayer(
//           urlTemplate: 'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
//           userAgentPackageName: 'com.aquacomp.ufrpe.donde.estas',
//         ),
//         _rotaUFRPE1(),
//         _rotaUFRPE2(),
//         _rotaUFRPE3(),
//         _paradasCircular(),
//         _circular(-8.017676098747222, -34.94868213586218),

//         // RichAttributionWidget(
//         //   attributions: [
//         //     TextSourceAttribution(
//         //       'OpenStreetMap contributors',
//         //       onTap: () {},
//         //     ),
//         //   ],
//         // ),
//       ],
//     );
//   }

//   Widget _circular(double latitude, double longitude) {
//     return MarkerLayer(markers: [
//       Marker(
//           point: LatLng(latitude, longitude),
//           width: 60,
//           height: 60,
//           child: Image.asset("assets/donde_estas_no_bg.png"))
//     ]);
//   }

//   Marker busTopLocation(double latitude, double longitude, String title) {
//     return Marker(
//       point: LatLng(latitude, longitude),
//       width: 120,
//       height: 100,
//       child: Column(
//         children: [
//           Text(
//             title,
//             style:
//                 const TextStyle(color: Colors.red, fontWeight: FontWeight.bold),
//           ),
//           const Icon(
//             Icons.location_on,
//             color: Colors.red,
//             size: 40,
//           )
//         ],
//       ),
//     );
//   }

//   Widget _paradasCircular() {
//     return MarkerLayer(markers: [
//       busTopLocation(-8.019607525549862, -34.94885169337855, "PREFEITURA"),
//       busTopLocation(-8.014510678463798, -34.95038975924049, "REITORIA"),
//       busTopLocation(-8.019992704527942, -34.95408284097032, "ZOOTECNIA"),
//       busTopLocation(-8.015771649845254, -34.95069933307541, "PISCINA"),
//       busTopLocation(-8.01759676387874, -34.949987690751506, "CEGOE"),
//       busTopLocation(-8.01780538024331, -34.944773647604364, "COMPUTAÇÃO"),
//       busTopLocation(-8.019475968963526, -34.94425489151376, "PESCA"),
//       busTopLocation(-8.016693139587256, -34.945152564873084, "FITOSANIDADE"),
//     ]);
//   }

//   Widget _rotaUFRPE1() {
//     return PolylineLayer(
//       polylines: [
//         Polyline(
//           points: const [
//             LatLng(-8.01667792104917, -34.94514980493042),
//             LatLng(-8.020248217878478, -34.94399675129072),
//             LatLng(-8.020203134434581, -34.94364096637195),
//             LatLng(-8.019925185718341, -34.94341449777967),
//             LatLng(-8.018631737512148, -34.94382487930449),
//             LatLng(-8.019571051349415, -34.94783263070886),
//             LatLng(-8.020252038191796, -34.94863784316253),
//             LatLng(-8.020487260936383, -34.9492662730964),
//             LatLng(-8.020160594453612, -34.94959110339133),
//             LatLng(-8.020463812548853, -34.950056800281466),
//             LatLng(-8.020114471220992, -34.951479399121524),
//             LatLng(-8.019834113952857, -34.95190087696983),
//             LatLng(-8.019430407069537, -34.95240513333515),
//             LatLng(-8.019897859100608, -34.953885711910885),
//             LatLng(-8.020027034417238, -34.953778181826635),
//             LatLng(-8.020105032233172, -34.95384204792062),
//             LatLng(-8.020071303442743, -34.95407835254548),
//             LatLng(-8.02024323218015, -34.95422346773798),
//             LatLng(-8.020015362155693, -34.95445633212201),
//             LatLng(-8.020716690119988, -34.954986464455196),
//             LatLng(-8.022085758814239, -34.953651229912694),
//             LatLng(-8.022076825480664, -34.95297459404283),
//             LatLng(-8.020129303159463, -34.95146794773644),
//           ],
//           color: Colors.black,
//           strokeWidth: 6,
//         ),
//       ],
//     );
//   }

//   Widget _rotaUFRPE2() {
//     return PolylineLayer(
//       polylines: [
//         Polyline(
//           points: const [
//             LatLng(-8.019748132014328, -34.948048347366516),
//             LatLng(-8.019257218661046, -34.94843224359644),
//             LatLng(-8.018907924851318, -34.94852924127782),
//             LatLng(-8.01795639419258, -34.94847966108866),
//             LatLng(-8.01717917107682, -34.949183366476454),
//             LatLng(-8.017143438280518, -34.94953521751632),
//             LatLng(-8.016643154310126, -34.949950221222686),
//             LatLng(-8.015999926760832, -34.94922847276465),
//             LatLng(-8.015553238717551, -34.9484706395785),
//             LatLng(-8.01537720408978, -34.94816501749126),
//             LatLng(-8.014949016200692, -34.94765079952151),
//             LatLng(-8.014583299793445, -34.947693086222564),
//             LatLng(-8.014247704049016, -34.948071065908124),
//             LatLng(-8.014096599247697, -34.94850312213283),
//             LatLng(-8.014027580257922, -34.94866674662845),
//             LatLng(-8.014081858778802, -34.949568489638416),
//             LatLng(-8.014155104113259, -34.949848239340284),
//             LatLng(-8.014240685601148, -34.95008527606954),
//             LatLng(-8.014321693998165, -34.950204634369165),
//             LatLng(-8.014468378410514, -34.950347912125274),
//             LatLng(-8.014637035114289, -34.95046324711286),
//             LatLng(-8.01477016401266, -34.95051833367607),
//             LatLng(-8.014938156586966, -34.95055588460198),
//             LatLng(-8.015130035721274, -34.95056654811759),
//             LatLng(-8.01541954041229, -34.95055313705993),
//             LatLng(-8.015540388634887, -34.95056386592162),
//             LatLng(-8.015642656169891, -34.950594908951786),
//             LatLng(-8.016348213411002, -34.950965402955426),
//             LatLng(-8.01653356402598, -34.9508869294485),
//             LatLng(-8.016654904670625, -34.95086248194982),
//             LatLng(-8.017127785168142, -34.95032442244919),
//             LatLng(-8.01755543493397, -34.949981109896285),
//             LatLng(-8.01786593748929, -34.95037400442833),
//             LatLng(-8.018327941806648, -34.95000037224711),
//             LatLng(-8.018566456236854, -34.95021279261025),
//             LatLng(-8.019588122389706, -34.949795538344134),
//             LatLng(-8.019869831217262, -34.949793642306524),
//             LatLng(-8.02016844239924, -34.94958311876435),
//           ],
//           color: Colors.black,
//           strokeWidth: 6,
//         ),
//       ],
//     );
//   }

//   Widget _rotaUFRPE3() {
//     return PolylineLayer(
//       polylines: [
//         Polyline(
//           points: const [
//             LatLng(-8.018327941806648, -34.95000037224711),
//             LatLng(-8.019687362354393, -34.948770781354874),
//           ],
//           color: Colors.black,
//           strokeWidth: 6,
//         ),
//       ],
//     );
//   }
// }
