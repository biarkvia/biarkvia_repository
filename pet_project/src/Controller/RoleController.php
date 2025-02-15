<?php

namespace App\Controller;

use App\Entity\Role;
use App\Form\CreateRoleType;
use App\Form\RemoveType;
use App\Operation\RoleOperation;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;

class RoleController extends AbstractController
{
    private RoleOperation $roleOperation;

    public function __construct(RoleOperation $roleOperation)
    {
        $this->roleOperation = $roleOperation;
    }

    #[Route('/roles', name: 'roles')]
    public function index(Request $request): Response
    {
        $criteria = [
            'title' => $request->query->get('title'),
            'description' => $request->query->get('description'),
            'isVisible' => $request->query->get('isVisible'),
        ];

        $criteria = array_filter($criteria, fn($value) => $value !== null && $value !== '');

        $page = (int) $request->query->get('page', 1);
        $limit = 4;
        $offset = max(($page - 1) * $limit, 0);

        $allFilteredRoles = $this->roleOperation->search($criteria);
        $totalRoles = count($allFilteredRoles);
        $roles = array_slice($allFilteredRoles, $offset, $limit);
        $totalPages = (int) ceil($totalRoles / $limit);

        return $this->render('role/role.html.twig', [
            'roles' => $roles,
            'currentPage' => $page,
            'totalPages' => $totalPages,
        ]);
    }

    #[Route('/roles/create', name: 'create_role')]
    public function create(Request $request): Response
    {
        $role = new Role();
        $form = $this->createForm(CreateRoleType::class, $role);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->roleOperation->store($role);
            return $this->redirectToRoute('roles', [
                'message' => 'Nová role byla úspěšně vytvořena.',
            ]);
        }

        return $this->render('role/create_role.html.twig', [
            'form' => $form->createView(),
        ]);
    }

    #[Route('/roles/edit/{id}', name: 'edit_role')]
    public function edit(int $id, Request $request): Response
    {
        $role = $this->roleOperation->findById($id);

        if (!$role) {
            return $this->redirectToRoute('error', [
                'message' => 'Role nebyla nalezena.',
            ]);
        }

        $form = $this->createForm(CreateRoleType::class, $role, ['is_edit' => true]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->roleOperation->store($role);
            return $this->redirectToRoute('roles', [
                'message' => 'Role byla úspěšně upravena.',
            ]);
        }

        return $this->render('role/edit_role.html.twig', [
            'form' => $form->createView(),
            'role' => $role,
        ]);
    }

    #[Route('/roles/remove/{id}', name: 'remove_role')]
    public function remove(int $id, Request $request): Response
    {
        $role = $this->roleOperation->findById($id);

        if (!$role) {
            return $this->redirectToRoute('error', [
                'message' => 'Role nebyla nalezena.',
            ]);
        }

        $form = $this->createForm(RemoveType::class, null, [
            'submit_label' => 'Smazat roli',
            'button_class' => 'btn-danger',
        ]);
        $form->handleRequest($request);

        if ($form->isSubmitted() && $form->isValid()) {
            $this->roleOperation->delete($role);
            return $this->redirectToRoute('roles', [
                'message' => 'Role byla úspěšně odstraněna.',
            ]);
        }

        return $this->render('role/remove_role.html.twig', [
            'form' => $form->createView(),
            'role' => $role,
        ]);
    }
}
