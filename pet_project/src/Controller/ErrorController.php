<?php

namespace App\Controller;

use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class ErrorController extends AbstractController
{
    #[Route('/error/{message}', name: 'error')]
    public function show(string $message = 'Tato stránka zatím neexistuje.'): Response
    {
        return $this->render('error/error.html.twig', [
            'message' => $message,
        ]);
    }
}